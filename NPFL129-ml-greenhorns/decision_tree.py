#!/usr/bin/env python3
import argparse
from queue import PriorityQueue
import numpy as np
import sklearn.datasets
import sklearn.metrics
import sklearn.model_selection

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--criterion", default="gini", type=str, help="Criterion to use; either `gini` or `entropy`")
parser.add_argument("--dataset", default="wine", type=str, help="Dataset to use")
parser.add_argument("--max_depth", default=None, type=int, help="Maximum decision tree depth")
parser.add_argument("--max_leaves", default=None, type=int, help="Maximum number of leaf nodes")
parser.add_argument("--min_to_split", default=2, type=int, help="Minimum examples required to split")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.25, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.

def calc_score(data, target, args):
    c = np.bincount(target)
    p = c / np.sum(c)

    if args.criterion == "gini":
        return len(data) * np.sum(p - p * p)
    else:
        p = p[p != 0]
        return len(data) * (-1) * np.sum(p * np.log(p))


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

    def can_split(self, args):
        if args.max_depth is not None and self.depth >= args.max_depth:
            return False
        if args.min_to_split > len(self.data):
            return False
        return calc_score(self.data, self.target, args) != 0

    def predict(self, d):
        if self.left and self.right:
            if d[self.split_feature] < self.split_value:
                return self.left.predict(d)
            return self.right.predict(d)
        return np.bincount(self.target).argmax()

    def crit_diff(self, args):
        crit = calc_score(self.data, self.target, args)
        max_d = -1
        fi = fv = None
        for feature_index in range(self.data.shape[1]):

            sorted = np.argsort(self.data[:, feature_index])

            x, index = np.unique(self.data[sorted][:, feature_index], return_index=True)
            index = sorted[index]

            for i in range(len(index) - 1):
                mean = (self.data[ index[i], feature_index] + self.data[index[i + 1], feature_index]) / 2

                score = calc_score(*get_left(self.data, self.target, feature_index, mean), args)
                score += calc_score(*get_right(self.data, self.target, feature_index, mean), args)

                diff = crit - score
                if diff > max_d:
                    max_d = diff
                    fi, fv = feature_index, mean

        return max_d, fi, fv

    def split(self, args, split_feature=None, split_value=None):

        if split_feature is None or split_value is None:
            crit_diff, split_feature, split_value = self.crit_diff(args)

        self.split_feature, self.split_value = split_feature, split_value
        self.left = Node(*get_left(self.data, self.target, split_feature, split_value), self.depth + 1)
        self.right = Node(*get_right(self.data, self.target, split_feature, split_value), self.depth + 1)

        return True


def make_tree(train_data, train_target, args):
    root = Node(train_data, train_target, 0)

    if not args.max_leaves:
        que = [root]
        while len(que) > 0:
            node = que.pop(0)
            if not node.can_split(args):
                continue
            if node.split(args):
                que.append(node.right)
                que.append(node.left)

        return root

    que = PriorityQueue()
    que.put((*root.crit_diff(args), root))
    leaves = 1
    while not que.empty() and leaves < args.max_leaves:
        score_diff, split_feature, split_value, node = que.get()

        if not node.can_split(args): continue
        if node.split(args, split_feature, split_value):
            l = node.left.crit_diff(args)
            r = node.right.crit_diff(args)
            que.put((l[0] * -1, l[1], l[2], node.left))
            que.put((r[0] * -1, r[1], r[2], node.right))
            leaves += 1
    return root

def main(args: argparse.Namespace) -> tuple[float, float]:
    # Use the given dataset.
    data, target = getattr(sklearn.datasets, "load_{}".format(args.dataset))(return_X_y=True)

    # Split the data randomly to train and test using `sklearn.model_selection.train_test_split`,
    # with `test_size=args.test_size` and `random_state=args.seed`.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    # TODO: Manually create a decision tree on the training data.
    #
    # - For each node, predict the most frequent class (and the one with
    #   the smallest number if there are several such classes).
    #
    # - When splitting a node, consider the features in sequential order, then
    #   for each feature consider all possible split points ordered in ascending
    #   value, and perform the first encountered split decreasing the criterion
    #   the most. Each split point is an average of two nearest unique feature values
    #   of the instances corresponding to the given node (e.g., for four instances
    #   with values 1, 7, 3, 3, the split points are 2 and 5).
    #
    # - Allow splitting a node only if:
    #   - when `args.max_depth` is not `None`, its depth must be less than `args.max_depth`
    #     (depth of the root node is zero);
    #   - when `args.max_leaves` is not `None`, there are less than `args.max_leaves` leaves
    #     (a leaf is a tree node without children);
    #   - there are at least `args.min_to_split` corresponding instances;
    #   - the criterion value is not zero.
    #
    # - When `args.max_leaves` is `None`, use recursive (left descendants first, then
    #   right descendants) approach, splitting every node if the constraints are valid.
    #   Otherwise (when `args.max_leaves` is not `None`), repeatably split a leaf where the
    #   constraints are valid and the overall criterion value ($c_left + c_right - c_node$)
    #   decreases the most. If there are several such nodes, choose the one
    #   which was created sooner (a left child is considered to be created
    #   before a right child).
    root = make_tree(train_data, train_target, args)

    # TODO: Finally, measure the training and testing accuracy.

    train_accuracy = sklearn.metrics.accuracy_score(train_target, [root.predict(d) for d in train_data])
    test_accuracy = sklearn.metrics.accuracy_score(test_target, [root.predict(d) for d in test_data])

    return 100 * train_accuracy, 100 * test_accuracy


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    train_accuracy, test_accuracy = main(args)

    print("Train accuracy: {:.1f}%".format(train_accuracy))
    print("Test accuracy: {:.1f}%".format(test_accuracy))
