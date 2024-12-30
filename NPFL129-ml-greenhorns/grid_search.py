#!/usr/bin/env python3
import argparse

import numpy as np
import sklearn.datasets
import sklearn.linear_model
import sklearn.metrics
import sklearn.model_selection
import sklearn.pipeline
import sklearn.preprocessing

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
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.5, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.


def main(args: argparse.Namespace) -> float:
    # Load digit dataset
    dataset = sklearn.datasets.load_digits()
    dataset.target = dataset.target % 2


    data_train, data_test, target_train, target_test = sklearn.model_selection.train_test_split(
        dataset.data, dataset.target, test_size=args.test_size, random_state=args.seed)

    pipe = sklearn.pipeline.Pipeline([
        ('scaler', sklearn.preprocessing.MinMaxScaler()),
        ('poly', sklearn.preprocessing.PolynomialFeatures()),
        ('logreg', sklearn.linear_model.LogisticRegression(random_state=args.seed))
    ])

    params = {
        'poly__degree': [1, 2],
        'logreg__C': [0.01, 1, 100],
        'logreg__solver': ['lbfgs', 'sag']
    }

    fold = sklearn.model_selection.StratifiedKFold(5)

    model = sklearn.model_selection.GridSearchCV(estimator=pipe, param_grid=params, cv=fold, n_jobs=-1)
    model.fit(data_train, target_train)

    # Then, using `sklearn.model_selection.StratifiedKFold(5)`, evaluate crossvalidated
    # train performance of all combinations of the following parameters:
    # - polynomial degree: 1, 2
    # - LogisticRegression regularization C: 0.01, 1, 100
    # - LogisticRegression solver: lbfgs, sag
    #
    # For the best combination of parameters, compute the test set accuracy.
    #
    # The easiest way is to use `sklearn.model_selection.GridSearchCV`.
    test_accuracy = model.score(data_test, target_test)

    # If `model` is a fitted `GridSearchCV`, you can use the following code
    # to show the results of all the hyperparameter values evaluated:
    #   for rank, accuracy, params in zip(model.cv_results_["rank_test_score"],
    #                                     model.cv_results_["mean_test_score"],
    #                                     model.cv_results_["params"]):
    #       print("Rank: {:2d} Cross-val: {:.1f}%".format(rank, 100 * accuracy),
    #             *("{}: {:<5}".format(key, value) for key, value in params.items()))

    return test_accuracy * 100


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    test_accuracy = main(args)
    print("Test accuracy: {:.2f}".format(test_accuracy))
