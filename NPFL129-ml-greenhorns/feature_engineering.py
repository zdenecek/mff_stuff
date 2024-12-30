#!/usr/bin/env python3

# All team solutions **must** list **all** members of the team.
# The members must be listed using their ReCodEx IDs anywhere
# in a comment block in the source file (on a line beginning with `#`).
#
# You can find out ReCodEx ID in the URL bar after navigating
# to your User profile page. The ID has the following format:
# 845f1776-2554-11ec-986f-f39926f24a9c
# 7247ebd3-5343-4adc-97a7-814cac40db06

import argparse

import numpy as np
import sklearn.compose
import sklearn.datasets
import sklearn.model_selection
import sklearn.pipeline
import sklearn.preprocessing

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--dataset", default="diabetes", type=str, help="Standard sklearn dataset to load")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--test_size", default=0.5, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.


def main(args: argparse.Namespace) -> tuple[np.ndarray, np.ndarray]:
    dataset = getattr(sklearn.datasets, "load_{}".format(args.dataset))()

    data_train, data_test, target_train, target_test = sklearn.model_selection.train_test_split(
        dataset.data, dataset.target, test_size=args.test_size, random_state=args.seed)


    ar_int = []
    ar_float = []
    for i in range(data_train.shape[1]):
        feature = np.copy(data_train[:,i])
        np.mod(feature, 1, out=feature)
        is_int = np.any(feature) == 0

        if is_int:
            ar_int.append(i)
        else:
            ar_float.append(i)


    ohe = sklearn.preprocessing.OneHotEncoder(sparse_output=False, handle_unknown="ignore")
    norm = sklearn.preprocessing.StandardScaler(with_mean=True, with_std=True)

    ct = sklearn.compose.ColumnTransformer(
        [("hot", ohe, ar_int), ("norm", norm, ar_float)]
    )

    polyn = sklearn.preprocessing.PolynomialFeatures(2, include_bias=False)

    pipe = sklearn.pipeline.Pipeline([("collumn_tran", ct), ("polyn", polyn)])
    pipe.fit(data_train, target_train)
    dtrain = pipe.transform(data_train)
    dtest = pipe.transform(data_test)

    return dtrain[:5], dtest[:5]


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    train_data, test_data = main(args)
    for dataset in [train_data, test_data]:
        for line in range(min(dataset.shape[0], 5)):
            print(" ".join("{:.4g}".format(dataset[line, column]) for column in range(min(dataset.shape[1], 140))))
