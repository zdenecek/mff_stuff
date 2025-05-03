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



parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="mnist_competition3.model", type=str, help="Model path")

def create_pipeline():
    from sklearn.pipeline import Pipeline
    from sklearn.preprocessing import StandardScaler
    from sklearn.neural_network import MLPClassifier

    pipeline = Pipeline([
        ('scaler', StandardScaler()),
        ('mlp', MLPClassifier(
            hidden_layer_sizes=(512, 256, 128),
            activation='relu',
            solver='adam',
            max_iter=100,
            early_stopping=True,
            random_state=42))
    ])
    return pipeline

class Dataset:
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



def main(args: argparse.Namespace) -> Optional[npt.ArrayLike]:
    if args.predict is None:


        # We are training a model.
        np.random.seed(args.seed)
        dataset = Dataset()
        data, target = dataset.data, dataset.target

        print("training...")
        # TODO: Train a model on the given dataset and store it in `model`.

        model = create_pipeline()
        model.fit(data,target)

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
