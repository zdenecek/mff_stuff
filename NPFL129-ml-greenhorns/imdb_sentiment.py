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
import sklearn.feature_extraction
import sklearn.metrics
import sklearn.model_selection
import sklearn.linear_model

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="imdb_sentiment.model", type=str, help="Model path")
# TODO: Add other arguments (typically hyperparameters) as you need.


class Dataset:
    """IMDB dataset.

    This is a modified IMDB dataset for sentiment classification. The text is
    already tokenized and partially normalized.
    """
    def __init__(self,
                 name="imdb_train.txt",
                 url="https://ufal.mff.cuni.cz/~courses/npfl129/2425/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            urllib.request.urlretrieve(url + name, filename="{}.tmp".format(name))
            os.rename("{}.tmp".format(name), name)

        # Load the dataset and split it into `data` and `target`.
        self.data = []
        self.target = []
        with open(name) as f_imdb:
            for line in f_imdb:
                label, text = line.split("\t", 1)
                self.data.append(text)
                self.target.append(int(label))


def load_word_embeddings(
        name="imdb_embeddings.npz",
        url="https://ufal.mff.cuni.cz/~courses/npfl129/2425/datasets/"):
    """Load word embeddings.

    These are selected word embeddings from FastText. For faster download, it
    only contains words that are in the IMDB dataset.
    """
    if not os.path.exists(name):
        print("Downloading embeddings {}...".format(name), file=sys.stderr)
        urllib.request.urlretrieve(url + name, filename="{}.tmp".format(name))
        os.rename("{}.tmp".format(name), name)

    with open(name, "rb") as f_emb:
        data = np.load(f_emb)
        words = data["words"]
        vectors = data["vectors"]
    embeddings = {word: vector for word, vector in zip(words, vectors)}
    return embeddings


def main(args: argparse.Namespace) -> Optional[npt.ArrayLike]:
    if args.predict is None:
        np.random.seed(args.seed)
        train = Dataset()

        print("Preprocessing dataset.", file=sys.stderr)
        vectorizer = sklearn.feature_extraction.text.TfidfVectorizer(
            ngram_range=(1, 3),    
            max_features=150000,    
            stop_words=None,       
            sublinear_tf=True,    
            min_df=5,               
            max_df=0.8              
        )
        train_x = vectorizer.fit_transform(train.data)

        from sklearn.feature_selection import SelectKBest, chi2
        selector = SelectKBest(chi2, k=50000)
        train_x = selector.fit_transform(train_x, train.target)

        train_x, validation_x, train_y, validation_y = sklearn.model_selection.train_test_split(
            train_x, train.target, test_size=0.25, random_state=args.seed)

        print("Training.", file=sys.stderr)
        from sklearn.svm import LinearSVC
        from sklearn.model_selection import GridSearchCV

        param_grid = {
            'C': [0.01, 0.1, 1, 10],
            'max_iter': [1000, 2000]
        }
        svc = LinearSVC(random_state=args.seed)
        grid_search = GridSearchCV(svc, param_grid, cv=3, n_jobs=-1)
        grid_search.fit(train_x, train_y)
        model = grid_search.best_estimator_

        print("Evaluation.", file=sys.stderr)
        validation_predictions = model.predict(validation_x)
        validation_accuracy = sklearn.metrics.accuracy_score(validation_y, validation_predictions)
        print("Validation accuracy {:.2f}%".format(100 * validation_accuracy))

        with lzma.open(args.model_path, "wb") as model_file:
            pickle.dump((model, vectorizer, selector), model_file)

    else:
        test = Dataset(args.predict)

        with lzma.open(args.model_path, "rb") as model_file:
            model, vectorizer, selector = pickle.load(model_file)

        test_x = vectorizer.transform(test.data)
        test_x = selector.transform(test_x)

        predictions = model.predict(test_x)
        return predictions

if __name__ == "__main__":
    main_args = parser.parse_args([] if "__file__" not in globals() else None)
    main(main_args)