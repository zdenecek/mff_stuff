#!/usr/bin/env python3
import argparse

import numpy as np
import sklearn.datasets
import sklearn.metrics
import sklearn.model_selection

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--bagging", default=False, action="store_true", help="Perform bagging")
parser.add_argument("--dataset", default="wine", type=str, help="Dataset to use")
parser.add_argument("--feature_subsampling", default=1.0, type=float, help="What fraction of features to subsample")
parser.add_argument("--max_depth", default=None, type=int, help="Maximum decision tree depth")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=44, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.25, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
parser.add_argument("--trees", default=1, type=int, help="Number of trees in the forest")
# If you add more arguments, ReCodEx will keep them with your default values.


def calc_score(data, target):
    c = np.bincount(target)
    p = c / c.sum()
    p = p[p != 0]
    return np.sum(p * np.log(p)) * (-1) * len(data)


def get_left(data, target, split_feature, split_value):
    return data[data[:, split_feature] < split_value], target[data[:, split_feature] < split_value]


def get_right(data, target, split_feature, split_value):
    return data[data[:, split_feature] >= split_value], target[data[:, split_feature] >= split_value]


class Node:
    def __init__(self, data, target, depth):
        self.data = data
        self.target = target
        self.depth = depth
        self.split_feature = self.split_value = None
        self.left = self.right = None


    def can_split(self, max_depth):
        if calc_score(self.data, self.target) == 0:
            return False
        if max_depth and self.depth >= max_depth:
            return False
        if 2 > len(self.data):
            return False

        return True
    
    def split(self, features_to_sample) -> bool:
        crit = calc_score(self.data, self.target)
        max_diff = -1
        max_diff_feature_index = max_diff_feature_value = None
        for feature_index in range(self.data.shape[1]):
            if not features_to_sample[feature_index]:
                continue

            sorted_idx = np.argsort(self.data[:, feature_index])

            x, index = np.unique(self.data[sorted_idx][:, feature_index], return_index=True)
            index = sorted_idx[index]

            for i in range(len(index) - 1):
                idx = index[i]
                next_idx = index[i + 1]
                mean = (self.data[idx, feature_index] + self.data[next_idx, feature_index]) / 2

                score = calc_score(*get_left(self.data, self.target, feature_index, mean))
                score += calc_score(*get_right(self.data, self.target, feature_index, mean))

                diff = crit - score
                if diff > max_diff:
                    max_diff = diff
                    max_diff_feature_index, max_diff_feature_value = feature_index, mean

        diff, split_feature, split_value = max_diff, max_diff_feature_index, max_diff_feature_value

        if diff == -1:
            return False
        self.split_feature, self.split_value = split_feature, split_value

        self.left = Node(*get_left(self.data, self.target, split_feature, split_value), self.depth + 1)
        self.right = Node(*get_right(self.data, self.target, split_feature, split_value), self.depth + 1)

        return True

    def predict(self, instance):
        if self.left or self.right:
            if instance[self.split_feature] < self.split_value:
                return self.left.predict(instance)
            return self.right.predict(instance)
        return np.bincount(self.target).argmax()


def main(args: argparse.Namespace) -> tuple[float, float]:
    # Use the given dataset.
    data, target = getattr(sklearn.datasets, "load_{}".format(args.dataset))(return_X_y=True)

    # Split the data randomly to train and test using `sklearn.model_selection.train_test_split`,
    # with `test_size=args.test_size` and `random_state=args.seed`.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    # Create random generators.
    generator_feature_subsampling = np.random.RandomState(args.seed)

    generator_feature_subsampling = np.random.RandomState(args.seed)
    def subsample_features(number_of_features: int) -> np.ndarray:
        mask = np.zeros(number_of_features, dtype=bool)
        selected_indices = generator_feature_subsampling.choice(
            number_of_features, size=int(args.feature_subsampling * number_of_features), replace=False)
        mask[selected_indices] = True
        return mask

    generator_bootstrapping = np.random.RandomState(args.seed)

    def bootstrap_dataset(train_data: np.ndarray) -> np.ndarray:
        return generator_bootstrapping.choice(len(train_data), size=len(train_data), replace=True)

    # TODO: Create a random forest on the training data.
    #
    # Use a simplified decision tree from the `decision_tree` assignment:
    # - use `entropy` as the criterion
    # - use `max_depth` constraint, to split a node only if:
    #   - its depth is less than `args.max_depth`
    #   - the criterion is not 0 (the corresponding instance targets are not the same)
    # When splitting nodes, proceed in the depth-first order, splitting all nodes
    # in the left subtree before the nodes in right subtree.
    #
    # Additionally, implement:
    # - feature subsampling: when searching for the best split, try only
    #   a subset of features. Notably, when splitting a node (i.e., when the
    #   splitting conditions [depth, criterion != 0] are satisfied), start by
    #   generating a feature mask using
    #     subsample_features(number_of_features)
    #   which gives a boolean value for every feature, with `True` meaning the
    #   feature is used during best split search, and `False` it is not
    #   (i.e., when `feature_subsampling == 1`, all features are used).
    #
    # - train a random forest consisting of `args.trees` decision trees
    #
    # - if `args.bagging` is set, before training each decision tree
    #   create a bootstrap sample of the training data by calling
    #     dataset_indices = bootstrap_dataset(train_data)
    #   and if `args.bagging` is not set, use the original training data.
    #
    # During prediction, use voting to find the most frequent class for a given
    # input, choosing the one with the smallest class number in case of a tie.

    # make forrest

    forrest = []
    for _ in range(args.trees):
        x = bootstrap_dataset(train_data) if args.bagging else list(range(len(train_data)))
        root = Node(train_data[x], train_target[x], 0)
        forrest.append(root)
        que = [root]
        while len(que) > 0:
            node = que.pop()
            if not node.can_split(max_depth=args.max_depth):
                continue
            success = node.split(subsample_features(data.shape[1]))
            if success:
                que.append(node.right)
                que.append(node.left)

    def calc_preds(forrest, data):
        return [np.argmax(np.bincount([t.predict(d) for t in forrest])) for d in data]

    train_predictions = calc_preds(forrest, train_data)
    test_predictions = calc_preds(forrest, test_data)

    # TODO: Finally, measure the training and testing accuracy.
    train_accuracy, test_accuracy = sklearn.metrics.accuracy_score(train_target,
                                                                   train_predictions), sklearn.metrics.accuracy_score(
        test_target, test_predictions)

    return 100 * train_accuracy, 100 * test_accuracy


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    train_accuracy, test_accuracy = main(args)

    print("Train accuracy: {:.1f}%".format(train_accuracy))
    print("Test accuracy: {:.1f}%".format(test_accuracy))
