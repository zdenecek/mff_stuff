#!/usr/bin/env python3
# Team members not sleeping to solve the Machine Learning mystery:
# Patrik Vacal: 5266924e-2ed4-11ed-8d67-d30fcedff11b
# Zdenek Tomis : 845f1776-2554-11ec-986f-f39926f24a9c
# Marek Seltenhofer: 7247ebd3-5343-4adc-97a7-814cac40db06
import argparse
import lzma
import pickle
import os
import re
import urllib.request
import sys
from typing import Optional

import numpy as np
import numpy.typing as npt
import sklearn
import sklearn.pipeline
import sklearn.ensemble
import sklearn.neural_network
import sklearn.naive_bayes
from sklearn.base import TransformerMixin
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.model_selection import GridSearchCV

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="isnt_it_ironic.model", type=str, help="Model path")
parser.add_argument("--test_size", default=0.2, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")

class DenseTransformer(TransformerMixin):
    def fit(self, X, y=None, **fit_params):
        return self

    def transform(self, X, y=None, **fit_params):
        return np.asarray( X.todense())


class EmojiMultiplierTransformer(TransformerMixin):
    def __init__(self, multiplier=2):
        self.multiplier = multiplier

    def fit(self, X, y=None, **fit_params):
        return self

    def transform(self, X, y=None, **fit_params):
        a = []

        for text in X:
            new_text = re.sub(r"[\U00010000-\U0010ffff]",
                              r"\g<0> " * self.multiplier,
                              text)
            a.append(new_text)

        return a



class Dataset:
    def __init__(self,
                 name="isnt_it_ironic.train.txt",
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            licence_name = name.replace(".txt", ".LICENSE")
            urllib.request.urlretrieve(url + licence_name, filename=licence_name)
            urllib.request.urlretrieve(url + name, filename=name)

        # Load the dataset and split it into `data` and `target`.
        self.data = []
        self.target = []

        with open(name, "r", encoding="utf-8-sig") as dataset_file:
            for line in dataset_file:
                label, text = line.rstrip("\n").split("\t")
                self.data.append(text)
                self.target.append(int(label))
        self.target = np.array(self.target, np.int32)


def main(args: argparse.Namespace) -> Optional[npt.ArrayLike]:
    if args.predict is None:
        # We are training a model.
        np.random.seed(args.seed)
        dataset = Dataset()

        if args.test_size == 0:
            X_train, y_train = dataset.data, dataset.target
            X_test, y_test = [], []
        else:
            X_train, X_test, y_train, y_test = sklearn.model_selection.train_test_split(dataset.data, dataset.target,
                test_size=args.test_size,
                random_state=args.seed
            )

        # DONE: Train a model on the given dataset and store it in `model`.
        pipe = sklearn.pipeline.Pipeline([
            # ('emoji', EmojiMultiplierTransformer(multiplier=5)),
            ('tfidf', TfidfVectorizer()),
            ('to_dense', DenseTransformer()),
            # ('model', sklearn.naive_bayes.GaussianNB())
            # ('model', sklearn.naive_bayes.BernoulliNB())
            # ('model', sklearn.naive_bayes.MultinomialNB())
            # ('model', sklearn.svm.SVC())
            ('ensamble', sklearn.ensemble.BaggingClassifier(sklearn.naive_bayes.BernoulliNB(), n_estimators=25, n_jobs=1))
        ])

        pipe.fit(dataset.data, dataset.target)

        # hyperparams = [{
        #     'ensamble__n_estimators': [1, 5, 10, 25, 50],
        # }]
        #
        # grid = GridSearchCV(pipe,
        #                       param_grid=hyperparams,
        #                       scoring='accuracy',
        #                       cv=5,
        #                       verbose=3)
        # grid.fit(dataset.data, dataset.target)
        # print(f"Best score: {grid.best_score_}, Best params: {grid.best_params_}")

        # test_accuracy = grid.score(X_test, y_test)

        def evaluate(model, X, y):
            y_predicted = model.predict(X)
            return sklearn.metrics.f1_score(y, y_predicted, average="binary")

        print("Train F1 score: {:.2f}".format(100 * evaluate(pipe, X_train, y_train)))
        print("Test F1 score: {:.2f}".format(100 * evaluate(pipe, X_test, y_test)))

        # print("Train F1 score: {:.2f}".format(100 * evaluate(grid, X_train, y_train)))
        # print("Test F1 score: {:.2f}".format(100 * evaluate(grid, X_test, y_test)))

        # Serialize the model.
        with lzma.open(args.model_path, "wb") as model_file:
            pickle.dump(pipe, model_file)

        print("[DONE]")

    else:
        # Use the model and return test set predictions.
        test = Dataset(args.predict)

        with lzma.open(args.model_path, "rb") as model_file:
            pipe = pickle.load(model_file)

        # DONE: Generate `predictions` with the test set predictions, either
        # as a Python list or a NumPy array.
        predictions = pipe.predict(test.data)

        return predictions


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    main(args)
