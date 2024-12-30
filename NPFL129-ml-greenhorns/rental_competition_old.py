#!/usr/bin/env python3
import argparse
import lzma
import os
import pickle
import sys
import urllib.request
from enum import Enum
from typing import Optional

import numpy as np
import numpy.typing as npt
import sklearn
import sklearn.compose
import sklearn.datasets
import sklearn.datasets
import sklearn.linear_model
import sklearn.metrics
import sklearn.model_selection
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

class Features(Enum):
    SEASON = 0
    YEAR = 1
    MONTH = 2
    HOUR = 3
    HOLIDAY = 4
    DAY = 5
    WORKING_DAY = 6
    WEATHER = 7
    TEMPERATURE = 8
    FEELING = 9
    RELATIVE = 10
    WINDSPEED = 11


class Dataset:
    """Rental Dataset.

    The dataset instances consist of the following 12 features:
    - season (1: winter, 2: spring, 3: summer, 4: autumn)
    - year (0: 2011, 1: 2012)
    - month (1-12)
    - hour (0-23)
    - holiday (binary indicator)
    - day of week (0: Sun, 1: Mon, ..., 6: Sat)
    - working day (binary indicator; a day is neither weekend nor holiday)
    - weather (1: clear, 2: mist, 3: light rain, 4: heavy rain)
    - temperature (normalized so that -8 Celsius is 0 and 39 Celsius is 1)
    - feeling temperature (normalized so that -16 Celsius is 0 and 50 Celsius is 1)
    - relative humidity (0-1 range)
    - windspeed (normalized to 0-1 range)

    The target variable is the number of rentals in the given hour.
    """

    def __init__(self,
                 name="rental_competition.train.npz",
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            urllib.request.urlretrieve(url + name, filename=name)

        # Load the dataset and return the data and targets.
        dataset = np.load(name)
        for key, value in dataset.items():
            setattr(self, key, value)



parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="rental_competition.model", type=str, help="Model path")


# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--test_size", default=0.2, type=lambda x: int(x) if x.isdigit() else float(x), help="Test size")


def create_model(data, target):
    lambdas = np.geomspace(0.01, 10, num=500)
    data_train, data_test, target_train, target_test = sklearn.model_selection.train_test_split(
        data, target, test_size=args.test_size, random_state=args.seed)

    best_mse, best_model = None, None
    for i in lambdas:
        model = sklearn.linear_model.Ridge(alpha=i)
        model.fit(data_train, target_train)
        pred_target = model.predict(data_test)

        mse = sklearn.metrics.root_mean_squared_error(target_test, pred_target)

        if best_mse is None or mse < best_mse:
            best_mse = mse
            best_model = model

    print(best_mse ** 0.5)
    return best_model


def create_pipeline():
    categorical = [
        (Features.SEASON, list(range(1, 5))),
        (Features.MONTH, list(range(1, 13))),
        (Features.DAY, list(range(7))),
        (Features.HOUR, list(range(24))),
        (Features.WEATHER, list(range(1, 5))),
        (Features.YEAR, list(range(2))),
    ]

    ohe = sklearn.preprocessing.OneHotEncoder(sparse_output=False, categories=[c[1] for c in categorical])

    sc = sklearn.preprocessing.StandardScaler(with_mean=True, with_std=True)

    normalize = [Features.TEMPERATURE.value, Features.FEELING.value, Features.RELATIVE.value,
                 Features.WINDSPEED.value, ]

    ct = sklearn.compose.ColumnTransformer([
        ("OneHotEncoderSeason", ohe, [c[0].value for c in categorical]),
        ("StandardScaler", sc, normalize)])

    pf = sklearn.preprocessing.PolynomialFeatures(2,include_bias=True)

    sgd = sklearn.linear_model.SGDRegressor()

    pipe = sklearn.pipeline.Pipeline([("ColumnTransformer", ct), ("PolynomialFeatures", pf), ("SgdRegressor", sgd)])
    return pipe


def main(args: argparse.Namespace) -> Optional[npt.ArrayLike]:
    if args.predict is None:
        # We are training a model.
        print("training...")

        np.random.seed(args.seed)
        dataset = Dataset()
        # TODO: Train a model on the given dataset and store it in `model`.
        pipe = create_pipeline()
        pipe.fit(dataset.data, dataset.target)

        p = pipe.predict(dataset.data)
        rmse = sklearn.metrics.root_mean_squared_error(dataset.target, p)
        print(rmse)

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
        predictions = model.predict(test.data)

        return predictions


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    main(args)
