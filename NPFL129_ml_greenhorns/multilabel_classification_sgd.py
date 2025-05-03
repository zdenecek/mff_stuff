#!/usr/bin/env python3
import argparse

import numpy as np
import sklearn.datasets
import sklearn.metrics
import sklearn.model_selection

# All team solutions **must** list **all** members of the team.
# The members must be listed using their ReCodEx IDs anywhere
# in a comment block in the source file (on a line beginning with `#`).
#
# You can find out ReCodEx ID in the URL bar after navigating
# to your User profile page. The ID has the following format:
# 845f1776-2554-11ec-986f-f39926f24a9c
# 7247ebd3-5343-4adc-97a7-814cac40db06

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--batch_size", default=10, type=int, help="Batch size")
parser.add_argument("--classes", default=5, type=int, help="Number of classes to use")
parser.add_argument("--data_size", default=200, type=int, help="Data size")
parser.add_argument("--epochs", default=9, type=int, help="Number of SGD training epochs")
parser.add_argument("--learning_rate", default=0.01, type=float, help="Learning rate")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.5, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.

def sigmoid(x) :
    return 1 / (1 + np.exp(-x))


def f1(target, pred, classes, average):
    TP = np.empty(classes)
    #TN = np.empty(classes)
    FP = np.empty(classes)
    FN = np.empty(classes)

    for i in range(classes):
        TP[i] = np.sum(np.logical_and(pred.T[i] == True, target.T[i] == True))
        #TN[i] = np.sum(np.logical_and(pred.T[i] == False, target.T[i] == False))
        FP[i] = np.sum(np.logical_and(pred.T[i] == True, target.T[i] == False))
        FN[i] = np.sum(np.logical_and(pred.T[i] == False, target.T[i] == True))


    if average == 'micro':
        TP = np.sum(TP)
        FP = np.sum(FP)
        FN = np.sum(FN)
        return (2 * TP) / (2 * TP + FP + FN)
    else:
        f1_metric = []
        f1_metric.append([(2 * TP[i]) / (2 * TP[i] + FP[i] + FN[i]) for i in range(classes)])
        return np.sum(f1_metric) / classes


def main(args: argparse.Namespace) -> tuple[np.ndarray, list[tuple[float, float]]]:
    # Create a random generator with a given seed.
    generator = np.random.RandomState(args.seed)

    # Generate an artificial classification dataset.
    data, target_list = sklearn.datasets.make_multilabel_classification(
        n_samples=args.data_size, n_classes=args.classes, allow_unlabeled=False,
        return_indicator=False, random_state=args.seed)

    # TODO: The `target` is a list of classes for every input example. Convert
    # it to a dense representation (n-hot encoding) -- for each input example,
    # the target should be vector of `args.classes` binary indicators.
    target = np.zeros((data.shape[0], args.classes))
    for i in range(len(target_list)):
        np.put(target[i], target_list[i], np.ones(data.shape[0]))


    # Append a constant feature with value 1 to the end of every input data.
    # Then we do not need to explicitly represent bias - it becomes the last weight.
    data = np.pad(data, [(0, 0), (0, 1)], constant_values=1)

    # Split the dataset into a train set and a test set.
    # Use `sklearn.model_selection.train_test_split` method call, passing
    # arguments `test_size=args.test_size, random_state=args.seed`.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    # Generate initial model weights.
    weights = generator.uniform(size=[train_data.shape[1], args.classes], low=-0.1, high=0.1)

    for epoch in range(args.epochs):
        permutation = generator.permutation(train_data.shape[0])

        # TODO: Process the data in the order of `permutation`. For every
        # `args.batch_size` of them, average their gradient, and update the weights.
        # You can assume that `args.batch_size` exactly divides `train_data.shape[0]`.
        for rng in range(0, train_data.shape[0], args.batch_size):
            r = permutation[rng:rng + args.batch_size]
            batch_data = np.copy(train_data[r, :])
            batch_target = np.copy(train_target[r])

            predictions = sigmoid(batch_data @ weights)

            #unit = np.zeros((batch_data.shape[0], args.classes))
            #for x in range(args.batch_size):
                #print(batch_target[x])
                #unit[x,batch_target[x]] = 1

            m = (predictions - batch_target)
            gradient = m.T  @ batch_data / args.batch_size

            weights -= args.learning_rate * gradient.T

        # TODO: After the SGD epoch, compute the micro-averaged and the
        # macro-averaged F1-score for both the train test and the test set.
        # Compute these scores manually, without using `sklearn.metrics`.

        train_predictions = sigmoid(train_data @ weights)
        train_predictions = train_predictions > 0.5

        test_predictions = sigmoid(test_data @ weights)
        test_predictions = test_predictions > 0.5

        train_f1_micro = f1(train_target, train_predictions, classes=args.classes, average='micro')
        train_f1_macro = f1(train_target, train_predictions, classes=args.classes, average='macro')
        test_f1_micro = f1(test_target, test_predictions, classes=args.classes, average='micro')
        test_f1_macro = f1(test_target, test_predictions, classes=args.classes, average='macro')


        print("After epoch {}: train F1 micro {:.2f}% macro {:.2f}%, test F1 micro {:.2f}% macro {:.1f}%".format(
            epoch + 1, 100 * train_f1_micro, 100 * train_f1_macro, 100 * test_f1_micro, 100 * test_f1_macro))

    return weights, [(100 * train_f1_micro, 100 * train_f1_macro), (100 * test_f1_micro, 100 * test_f1_macro)]


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    weights, metrics = main(args)
    print("Learned weights:",
          *(" ".join([" "] + ["{:.2f}".format(w) for w in row[:10]] + ["..."]) for row in weights.T), sep="\n")
