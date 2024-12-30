#!/usr/bin/env python3
import argparse
import lzma
import pickle
import os
import sys
import urllib.request

import numpy as np
import sklearn.linear_model
import sklearn.metrics
import sklearn.model_selection

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--idf", default=False, action="store_true", help="Use IDF weights")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=79, type=int, help="Random seed")
parser.add_argument("--tf", default=False, action="store_true", help="Use TF weights")
parser.add_argument("--test_size", default=500, type=int, help="Test set size")
parser.add_argument("--train_size", default=1000, type=int, help="Train set size")
# For these and any other arguments you add, ReCodEx will keep your default value.


class NewsGroups:
    def __init__(self,
                 name="20newsgroups.train.pickle",
                 data_size=None,
                 url="https://ufal.mff.cuni.cz/~courses/npfl129/2425/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            urllib.request.urlretrieve(url + name, filename="{}.tmp".format(name))
            os.rename("{}.tmp".format(name), name)

        with lzma.open(name, "rb") as dataset_file:
            dataset = pickle.load(dataset_file)

        self.DESCR = dataset.DESCR
        self.data = dataset.data[:data_size]
        self.target = dataset.target[:data_size]
        self.target_names = dataset.target_names


def main(args: argparse.Namespace) -> float:
    # Load the 20newsgroups data.
    newsgroups = NewsGroups(data_size=args.train_size + args.test_size)

    # Create train-test split.
    train_data, test_data, train_target, test_target = sklearn.model_selection.train_test_split(
        newsgroups.data, newsgroups.target, test_size=args.test_size, random_state=args.seed)

    import re
    from collections import Counter

    # Extract terms from the training data
    term_counts = Counter()
    term_pattern = re.compile(r'\w+')

    for doc in train_data:
        terms = term_pattern.findall(doc)
        term_counts.update(terms)

    # Create vocabulary of terms that appear at least twice
    vocab = [term for term, count in term_counts.items() if count >= 2]
    term_to_index = {term: idx for idx, term in enumerate(vocab)}
    vocab_size = len(vocab)
    
    print("Number of unique terms with at least two occurrences:", vocab_size)

    # Compute document frequencies
    df_counter = Counter()
    for doc in train_data:
        terms = set(term_pattern.findall(doc))
        for term in terms:
            if term in term_to_index:
                df_counter[term] += 1

    N = len(train_data)
    # Compute IDF vector
    idf = np.zeros(vocab_size)
    for term, idx in term_to_index.items():
        df_t = df_counter[term]
        idf[idx] = np.log(N / (df_t + 1))

    # TODO: For each document, compute its features as
    # - term frequency (TF), if `args.tf` is set (term frequency is
    #   proportional to the number of term occurrences but normalized to
    #   sum to 1 over all features of a document);
    # - otherwise, use binary indicators (1 if a given term is present, else 0)
    #
    # Then, if `args.idf` is set, multiply the document features by the
    # inverse document frequencies (IDF), where
    # - use the variant which contains `+1` in the denominator;
    # - the IDFs are computed on the train set and then reused without
    #   modification on the test set.

    X_train = np.zeros((len(train_data), vocab_size))
    X_test = np.zeros((len(test_data), vocab_size))

    # Process train data
    for i, doc in enumerate(train_data):
        x = np.zeros(vocab_size)
        terms = term_pattern.findall(doc)
        for term in terms:
            if term in term_to_index:
                idx = term_to_index[term]
                if args.tf:
                    x[idx] += 1
                else:
                    x[idx] = 1  

        if args.tf:
            if x.sum() > 0:
                x /= x.sum()

        if args.idf:
            x *= idf

        X_train[i] = x

    # Process test data
    for i, doc in enumerate(test_data):
        x = np.zeros(vocab_size)
        terms = term_pattern.findall(doc)
        for term in terms:
            if term in term_to_index:
                idx = term_to_index[term]
                if args.tf:
                    x[idx] += 1
                else:
                    x[idx] = 1  

        if args.tf:
            if x.sum() > 0:
                x /= x.sum()

        if args.idf:
            x *= idf  

        X_test[i] = x

    # TODO: Train a `sklearn.linear_model.LogisticRegression(solver="liblinear", C=10_000)`
    # model on the train set, and classify the test set.

    clf = sklearn.linear_model.LogisticRegression(solver="liblinear", C=10_000, random_state=args.seed)
    clf.fit(X_train, train_target)
    y_pred = clf.predict(X_test)

    # TODO: Evaluate the test set performance using a macro-averaged F1 score.
    f1_score = sklearn.metrics.f1_score(test_target, y_pred, average='macro')

    return 100 * f1_score



if __name__ == "__main__":
    main_args = parser.parse_args([] if "__file__" not in globals() else None)
    f1_score = main(main_args)
    print("F-1 score for TF={}, IDF={}: {:.1f}%".format(main_args.tf, main_args.idf, f1_score))