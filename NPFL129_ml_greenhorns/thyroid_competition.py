#!/usr/bin/env python3
import argparse
import lzma
import os
import pickle
import sys
from typing import Optional
import urllib.request

import numpy as np
import numpy.typing as npt

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
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=92, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="thyroid_competition.model", type=str, help="Model path")


class Dataset:
    """Thyroid Dataset.

    The dataset contains real medical data related to thyroid gland function,
    classified either as normal or irregular (i.e., some thyroid disease).
    The data consists of the following features in this order:
    - 15 binary features
    - 6 real-valued features

    The target variable is binary, with 1 denoting a thyroid disease and
    0 normal function.
    """

    def __init__(self,
                 name="thyroid_competition.train.npz",
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            urllib.request.urlretrieve(url + name, filename=name)

        # Load the dataset and return the data and targets.
        dataset = np.load(name)
        for key, value in dataset.items():
            setattr(self, key, value)

def add_interactions(X):
    X_interact = np.hstack([
        X,
        (X[:, [15]] * X[:, [16]]),  # Interaction between features 15 and 16
        (X[:, [17]] * X[:, [18]]),  # Interaction between features 17 and 18
        (X[:, [19]] * X[:, [20]])   # Interaction between features 19 and 20
    ])
    return X_interact

def create_pipeline():
    from sklearn.pipeline import Pipeline
    from sklearn.preprocessing import StandardScaler, FunctionTransformer
    from sklearn.compose import ColumnTransformer
    from sklearn.linear_model import LogisticRegression
    from sklearn.feature_selection import SelectFromModel
    from sklearn.model_selection import GridSearchCV

    real_features = list(range(15, 21))
    interaction_features = [21, 22, 23] 

    interaction_transformer = FunctionTransformer(add_interactions, validate=False)

    scaler = StandardScaler()
    preprocessor = ColumnTransformer(
        transformers=[
            ('scaler', scaler, real_features + interaction_features)
        ],
        remainder='passthrough'  # Pass through binary features without scaling
    )

    logistic = LogisticRegression(
        solver='saga',  
        max_iter=10000,
        random_state=42
    )

    pipe = Pipeline([
        ('interaction_terms', interaction_transformer),
        ('preprocessor', preprocessor),
        ('feature_selection', SelectFromModel(LogisticRegression(penalty='l1', solver='saga', random_state=42))),
        ('classifier', logistic)
    ])

    param_grid = {
        'classifier__penalty': ['l1', 'l2', 'elasticnet'],
        'classifier__C': [0.01, 0.1, 1, 10],
        'classifier__l1_ratio': [0.0, 0.5, 1.0],  # Only used if penalty='elasticnet'
        'feature_selection__threshold': ['mean', 'median', 0.1, 0.2],
    }

    grid_search = GridSearchCV(
        estimator=pipe,
        param_grid=param_grid,
        cv=5,
        scoring='accuracy',
        n_jobs=-1
    )

    return grid_search


def main(args: argparse.Namespace) -> Optional[npt.ArrayLike]:
    if args.predict is None:
        # We are training a model.
        np.random.seed(args.seed)
        dataset = Dataset()

        import sklearn.metrics

        # TODO: Train a model on the given dataset and store it in `model`.
        pipe = create_pipeline()
        pipe.fit(dataset.data, dataset.target)

        values = pipe.predict(dataset.data)
        predictions = values > 0.5

        accuracy = sklearn.metrics.accuracy_score(dataset.target, predictions)
        loss = sklearn.metrics.log_loss(dataset.target, values)

        print(f"{ round(accuracy * 100,2) }% loss { round(loss,4)}")

        model = pipe
        print("saving...")

        # Serialize the model.
        with lzma.open(args.model_path, "wb") as model_file:
            pickle.dump(model, model_file)

    else:
        # Use the model and return test set predictions, either as a Python list or a NumPy array.
        test = Dataset(args.predict)

        with lzma.open(args.model_path, "rb") as model_file:
            model = pickle.load(model_file)

        # TODO: Generate `predictions` with the test set predictions.
        values = model.predict(test.data)
        predictions = values

        return predictions


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    main(args)
