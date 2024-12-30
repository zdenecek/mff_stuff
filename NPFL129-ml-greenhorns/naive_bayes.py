#!/usr/bin/env python3
# Team members not sleeping to solve the Machine Learning mystery:
# Patrik Vacal: 5266924e-2ed4-11ed-8d67-d30fcedff11b
# Zdenek Tomis : 845f1776-2554-11ec-986f-f39926f24a9c
# Marek Seltenhofer: 7247ebd3-5343-4adc-97a7-814cac40db06
import argparse

import numpy as np
import scipy.stats

import sklearn.datasets
import sklearn.model_selection

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--alpha", default=0.1, type=float, help="Smoothing parameter of our NB classifier")
parser.add_argument("--naive_bayes_type", default="gaussian", type=str, help="NB type gaussian/multinomial/bernoulli")
parser.add_argument("--classes", default=10, type=int, help="Number of classes")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=72, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.5, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.

def main(args: argparse.Namespace) -> tuple[float, float]:
    data, target = sklearn.datasets.load_digits(n_class=args.classes, return_X_y=True)

    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    class_counts = np.bincount(train_target)
    class_priors = class_counts / len(train_target)

    if args.naive_bayes_type == "gaussian":
        means = np.zeros((args.classes, train_data.shape[1]))
        variances = np.zeros((args.classes, train_data.shape[1]))

        for c in range(args.classes):
            class_data = train_data[train_target == c]
            means[c] = class_data.mean(axis=0)
            variances[c] = class_data.var(axis=0) + args.alpha

        # Predict
        log_probs = np.zeros((len(test_data), args.classes))
        for c in range(args.classes):
            log_prior = np.log(class_priors[c])
            log_likelihood = np.sum(scipy.stats.norm.logpdf(
                test_data, loc=means[c], scale=np.sqrt(variances[c])), axis=1)
            log_probs[:, c] = log_prior + log_likelihood

    elif args.naive_bayes_type == "multinomial":
        counts = np.zeros((args.classes, train_data.shape[1]))
        for c in range(args.classes):
            counts[c] = train_data[train_target == c].sum(axis=0)
        probs = (counts + args.alpha) / (counts.sum(axis=1, keepdims=True) + args.alpha * train_data.shape[1])

        # Predict
        log_probs = np.zeros((len(test_data), args.classes))
        for c in range(args.classes):
            log_prior = np.log(class_priors[c])
            log_likelihood = np.sum(test_data * np.log(probs[c]), axis=1)
            log_probs[:, c] = log_prior + log_likelihood

    elif args.naive_bayes_type == "bernoulli":
        binarized_data = (train_data >= 8).astype(np.float64)
        binarized_test_data = (test_data >= 8).astype(np.float64)

        probabilities = np.zeros((args.classes, train_data.shape[1]))
        for c in range(args.classes):
            class_data = binarized_data[train_target == c]
            probabilities[c] = (class_data.sum(axis=0) + args.alpha) / (class_data.shape[0] + 2 * args.alpha)

        # Predict
        log_probs = np.zeros((len(test_data), args.classes))
        for c in range(args.classes):
            log_prior = np.log(class_priors[c])
            log_likelihood = np.sum(
                binarized_test_data * np.log(probabilities[c]) +
                (1 - binarized_test_data) * np.log(1 - probabilities[c]), axis=1)
            log_probs[:, c] = log_prior + log_likelihood

    predicted_classes = np.argmax(log_probs, axis=1)
    test_accuracy = np.mean(predicted_classes == test_target)

    joint_log_prob = np.sum(log_probs[np.arange(len(test_target)), test_target])

    return 100 * test_accuracy, joint_log_prob


if __name__ == "__main__":
    main_args = parser.parse_args([] if "__file__" not in globals() else None)
    test_accuracy, test_log_probability = main(main_args)

    print("Test accuracy {:.2f}%, log probability {:.2f}".format(test_accuracy, test_log_probability))
