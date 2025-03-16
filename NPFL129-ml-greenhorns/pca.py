#!/usr/bin/env python3
import argparse
import os
import sys
import urllib.request

import numpy as np
import sklearn.base
import sklearn.linear_model
import sklearn.model_selection
import sklearn.pipeline
import sklearn.preprocessing

# Team members not sleeping to solve the Machine Learning mystery:
# Zdenek Tomis : 845f1776-2554-11ec-986f-f39926f24a9c
# Aneta Kahleova: 4130a9b5-2532-11ec-986f-f39926f24a9c
# Marek Seltenhofer: 7247ebd3-5343-4adc-97a7-814cac40db06

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--data_size", default=5000, type=int, help="Data size")
parser.add_argument("--max_iter", default=1000, type=int, help="Maximum iterations for LR")
parser.add_argument("--pca", default=10, type=int, help="PCA dimensionality")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
parser.add_argument("--solver", default="lbfgs", type=str, help="LR solver")
parser.add_argument("--test_size", default=0.5, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")
# If you add more arguments, ReCodEx will keep them with your default values.


class MNIST:
    """MNIST Dataset.

    The train set contains 60000 images of handwritten digits. The data
    contain 28*28=784 values in the range 0-255, the targets are numbers 0-9.
    """
    def __init__(self,
                 name="mnist.train.npz",
                 data_size=None,
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            urllib.request.urlretrieve(url + name, filename=name)

        # Load the dataset, i.e., `data` and optionally `target`.
        dataset = np.load(name)
        for key, value in dataset.items():
            setattr(self, key, value[:data_size])
        self.data = self.data.reshape([-1, 28*28]).astype(float)


class PCATransformer(sklearn.base.TransformerMixin):
    def __init__(self, n_components, seed):
        self._n_components = n_components
        self._seed = seed

    def fit(self, X, y=None):
        generator = np.random.RandomState(self._seed)

        # DONE: Compute the `self._n_components` principal components
        # and store them as columns of `self._V` matrix.
        if self._n_components <= 10:
            # DONE: Use the power iteration algorithm for <= 10 dimensions.
            #
            # To compute every eigenvector, apply 10 iterations, and set
            # the initial value of every eigenvector to
            #   `generator.uniform(-1, 1, size=X.shape[1])`
            # Compute the vector norms using `np.linalg.norm`.

            self._V = np.zeros([self._n_components, X.shape[1]])
            u = np.mean(X, axis=0)
            S = (X - u).T @ (X - u) / X.shape[0]
            #S = np.cov(X.T)
            for i in range(self._n_components):
                v = generator.uniform(-1, 1, size=X.shape[1])
                for _ in range(10):
                    v = S @ v
                    delta = np.linalg.norm(v)
                    v /= delta
                #S -= delta * (v @ v.T)
                S -= delta * np.outer(v, v)
                self._V[i] = v

        else:
            # DONE: Use the SVD decomposition computed with `np.linalg.svd`
            # to find the principal components.
            X = X - X.mean(axis=0)
            u, s, vh = np.linalg.svd(X, full_matrices=False)
            self._V = vh[:self._n_components, :]

        # We round the principal components to avoid rounding errors during
        # ReCodEx evaluation.
        self._V = np.around(self._V, decimals=4)

        return self

    def transform(self, X):
        # DONE: Transform the given `X` using the precomputed `self._V`.
        return X @ self._V.T


def main(args: argparse.Namespace) -> float:
    # Use the MNIST dataset.
    dataset = MNIST(data_size=args.data_size)

    # Split the dataset into a train set and a test set.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        dataset.data, dataset.target, test_size=args.test_size, random_state=args.seed)

    pca = [("PCA", PCATransformer(args.pca, args.seed))] if args.pca else []

    pipeline = sklearn.pipeline.Pipeline([
        ("scaling", sklearn.preprocessing.MinMaxScaler()),
        *pca,
        ("classifier", sklearn.linear_model.LogisticRegression(
            solver=args.solver, max_iter=args.max_iter, random_state=args.seed)),
    ])
    pipeline.fit(train_data, train_target)

    test_accuracy = pipeline.score(test_data, test_target)
    return test_accuracy


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    accuracy = main(args)
    print("Test set accuracy: {:.2f}%".format(100 * accuracy))
