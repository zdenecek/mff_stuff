#!/usr/bin/env python3
import argparse

import numpy as np
import sklearn.datasets
import sklearn.metrics
import sklearn.model_selection

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--batch_size", default=10, type=int, help="Batch size")
parser.add_argument("--classes", default=10, type=int, help="Number of classes to use")
parser.add_argument("--epochs", default=10, type=int, help="Number of SGD training epochs")
parser.add_argument("--hidden_layer", default=20, type=int, help="Hidden layer size")
parser.add_argument("--learning_rate", default=0.01, type=float, help="Learning rate")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--test_size", default=797, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.

# All team solutions **must** list **all** members of the team.
# The members must be listed using their ReCodEx IDs anywhere
# in a comment block in the source file (on a line beginning with `#`).
#
# You can find out ReCodEx ID in the URL bar after navigating
# to your User profile page. The ID has the following format:
# 845f1776-2554-11ec-986f-f39926f24a9c
# 7247ebd3-5343-4adc-97a7-814cac40db06


def main(args: argparse.Namespace) -> tuple[tuple[np.ndarray, ...], list[float]]:
    # Create a random generator with a given seed.
    generator = np.random.RandomState(args.seed)

    # Load the digits dataset.
    data, target = sklearn.datasets.load_digits(n_class=args.classes, return_X_y=True)

    # Split the dataset into a train set and a test set.
    # Use `sklearn.model_selection.train_test_split` method call, passing
    # arguments `test_size=args.test_size, random_state=args.seed`.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    # Generate initial model weights.
    weights = [generator.uniform(size=[train_data.shape[1], args.hidden_layer], low=-0.1, high=0.1),
               generator.uniform(size=[args.hidden_layer, args.classes], low=-0.1, high=0.1)]
    biases = [np.zeros(args.hidden_layer), np.zeros(args.classes)]


    def accur(target, pred):
        good = 0
        for i in range(len(pred)):
            class_num = np.argmax(pred[i])

            if target[i] == class_num:
                good += 1
        accuracy = good / len(target)
        return accuracy

    def ReLU(x):
        return np.maximum(x, 0)

    def softmax(x):
        return np.exp(x)/sum(np.exp(x))


    def forward(inputs):
        # TODO: Implement forward propagation, returning *both* the value of the hidden
        # layer and the value of the output layer.
        #
        # We assume a neural network with a single hidden layer of size `args.hidden_layer`
        # and ReLU activation, where $ReLU(x) = max(x, 0)$, and an output layer with softmax
        # activation.
        #
        # The value of the hidden layer is computed as `ReLU(inputs @ weights[0] + biases[0])`.
        # The value of the output layer is computed as `softmax(hidden_layer @ weights[1] + biases[1])`.
        #
        # Note that you need to be careful when computing softmax, because the exponentiation
        # in softmax can easily overflow. To avoid it, you should use the fact that
        # $softmax(z) = softmax(z + any_constant)$ and compute $softmax(z) = softmax(z - maximum_of_z)$.
        # That way we only exponentiate values which are non-positive, and overflow does not occur.
        hidden_layer = ReLU(inputs @ weights[0] + biases[0])
        output_layer_without_activ = hidden_layer @ weights[1] + biases[1]
        output_layer = softmax(output_layer_without_activ - output_layer_without_activ.max())
        return hidden_layer, output_layer


    for epoch in range(args.epochs):
        permutation = generator.permutation(train_data.shape[0])

        grad_w_out = 0
        grad_w_hidden = 0
        grad_b_out = 0
        grad_b_hidden = 0

        for batch in range(0, train_data.shape[0], args.batch_size):
            perm_index = permutation[batch : batch + args.batch_size]
            batch_data = np.copy(train_data[perm_index,])
            batch_target = np.copy(train_target[perm_index])

            for i in range(batch_data.shape[0]):
                hidden_layer, predictions = forward(batch_data[i])

                target_distrib = np.zeros(args.classes)
                target_distrib[batch_target[i]] = 1

                respect_to_b_out = predictions - target_distrib
                grad_b_out += respect_to_b_out

                respect_to_w_out = np.outer(hidden_layer, respect_to_b_out)
                grad_w_out += respect_to_w_out

                respect_to_b_hidden = (hidden_layer > 0) * (weights[1] @ (predictions - target_distrib))
                grad_b_hidden += respect_to_b_hidden

                w_respect_to_w_hidden = np.outer(batch_data[i], respect_to_b_hidden)
                grad_w_hidden += w_respect_to_w_hidden


            weights[1] -= args.learning_rate*(grad_w_out / args.batch_size)
            weights[0] -= args.learning_rate*(grad_w_hidden / args.batch_size)
            biases[1] -= args.learning_rate*(grad_b_out / args.batch_size)
            biases[0] -= args.learning_rate*(grad_b_hidden / args.batch_size)

            grad_w_out = 0
            grad_w_hidden = 0
            grad_b_out = 0
            grad_b_hidden = 0




        # TODO: Process the data in the order of `permutation`. For every
        # `args.batch_size` of them, average their gradient, and update the weights.
        # You can assume that `args.batch_size` exactly divides `train_data.shape[0]`.
        #
        # The gradient used in SGD has now four parts, gradient of `weights[0]` and `weights[1]`
        # and gradient of `biases[0]` and `biases[1]`.
        #
        # You can either compute the gradient directly from the neural network formula,
        # i.e., as a gradient of $-log P(target | data)$, or you can compute
        # it step by step using the chain rule of derivatives, in the following order:
        # - compute the derivative of the loss with respect to *inputs* of the
        #   softmax on the last layer,
        # - compute the derivative with respect to `weights[1]` and `biases[1]`,
        # - compute the derivative with respect to the hidden layer output,
        # - compute the derivative with respect to the hidden layer input,
        # - compute the derivative with respect to `weights[0]` and `biases[0]`.

        # TODO: After the SGD epoch, measure the accuracy for both the
        # train test and the test set.
        train_pred = []
        for i in train_data:
            hid, pred = forward(i)
            train_pred.append(pred)

        test_pred = []
        for i in test_data:
            hid, pred = forward(i)
            test_pred.append(pred)

        #arr = [forward(i) for i in test_data]
        #train_accuracy, test_accuracy = accur(train_target, arr[0]), accur(test_target, arr[1])
        train_accuracy, test_accuracy = accur(train_target, train_pred), accur(test_target, test_pred)
        #train_accuracy, test_accuracy = accur(train_target, testpred), accur(test_target, testpred2)

        print("After epoch {}: train acc {:.1f}%, test acc {:.1f}%".format(
            epoch + 1, 100 * train_accuracy, 100 * test_accuracy))

    return tuple(weights + biases), [100 * train_accuracy, 100 * test_accuracy]


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    parameters, metrics = main(args)
    print("Learned parameters:",
          *(" ".join([" "] + ["{:.2f}".format(w) for w in ws.ravel()[:12]] + ["..."]) for ws in parameters), sep="\n")
