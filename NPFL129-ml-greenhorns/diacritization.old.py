#!/usr/bin/env python3
import argparse
import lzma
import os
import pickle
import sys
from typing import Optional
import urllib.request

# All team solutions **must** list **all** members of the team.
# The members must be listed using their ReCodEx IDs anywhere
# in a comment block in the source file (on a line beginning with `#`).
#
# You can find out ReCodEx ID in the URL bar after navigating
# to your User profile page. The ID has the following format:
# 845f1776-2554-11ec-986f-f39926f24a9c
# 7247ebd3-5343-4adc-97a7-814cac40db06

from multiprocessing import Pool

from sklearn.pipeline import Pipeline
from sklearn.linear_model import LogisticRegressionCV, SGDClassifier
from sklearn.preprocessing import FunctionTransformer, OneHotEncoder, StandardScaler, PolynomialFeatures
from sklearn.feature_extraction import DictVectorizer

import numpy as np

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=92, type=int, help="Random seed")
parser.add_argument("--text", default=None, type=str, help="string to translate")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="diacritization.model", type=str, help="Model path")
parser.add_argument("--save", default=False, action="store_true", help="save")
parser.add_argument("--verbose", default=False, action="store_true", help="save")

class Dataset:
    LETTERS_NODIA = "acdeeinorstuuyz"
    LETTERS_DIA = "áčďéěíňóřšťúůýž"

    # A translation table usable with `str.translate` to rewrite characters with dia to the ones without them.
    DIA_TO_NODIA = str.maketrans(LETTERS_DIA + LETTERS_DIA.upper(), LETTERS_NODIA + LETTERS_NODIA.upper())

    def __init__(self,
                 name="fiction-train.txt",
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            licence_name = name.replace(".txt", ".LICENSE")
            urllib.request.urlretrieve(url + licence_name, filename=licence_name)
            urllib.request.urlretrieve(url + name, filename=name)

        # Load the dataset and split it into `data` and `target`.
        with open(name, "r", encoding="utf-8-sig") as dataset_file:
            self.target = dataset_file.read()
        self.data = self.target.translate(self.DIA_TO_NODIA)


diacriticized_letters = "acdeinorstuyz"
diacriticized_letters = "aceinrstuyz"
target_labels = {
    "a": 0,
    "á": 1,
    "c": 0,
    "č": 1,
    "d": 0,
    "ď": 1,
    "e": 0,
    "é": 1,
    "ě": 2,
    "i": 0,
    "í": 1,
    "n": 0,
    "ň": 1,
    "o": 0,
    "ó": 1,
    "r": 0,
    "ř": 1,
    "s": 0,
    "š": 1,
    "t": 0,
    "ť": 1,
    "u": 0,
    "ú": 1,
    "ů": 2,
    "y": 0,
    "ý": 1,
    "z": 0,
    "ž": 1
}
inv_labels = {

    "a": {
         1: "á", },
    "c": {
         1: "č", },
    "d": {
         1: "ď", },
    "e": {
         1: "é",
         2: "ě", },
    "i": {
         1: "í", },
    "n": {
         1: "ň", },
    "o": {
         1: "ó", },
    "r": {
         1: "ř", },
    "s": {
         1: "š", },
    "t": {
         1: "ť", },
    "u": {
         1: "ú",
         2: "ů", },
    "y": {
         1: "ý", },
    "z": {
         1: "ž"
    }
}


import re


def clean_text(text):
    return re.sub(r"[^ a-zA-ZáčďéěíňóřšťúůýžÁČĎÉĚÍŇÓŘŠŤÚŮÝŽ]", "", text)


def extract_features(words, letter):
    """Extract features for all words and a specific letter."""
    data = []
    avg_word_len = sum(len(w) for w in words) / len(words) if words else 1

    for index, word in enumerate(words):
        for i, l in enumerate(word):
            if l != letter:
                continue

            suffix = word[-3:]
            prefix = word[:3]
            is_soft_consonant = word[i - 1] in "cčdďlňřsšťzž" if i > 0 else False

            data.append({
                # Basic features
                "word": word,
                "prev_letter": word[i - 1] if i != 0 else "",

                "pprev_letter": word[i - 2:i] if i > 1 else "",

                "ppprev_letter": word[i - 3:i] if i > 2 else "",

                "next_letter": word[i + 1] if i + 1 < len(word) else "",

                "word_len": len(word),
                "norm_len": len(word) / avg_word_len,
                "letter_index": i,
                "letters_after": len(word) - i,
                "prev_letter": word[i - 1] if i > 0 else "",
                "next_letter": word[i + 1] if i + 1 < len(word) else "",
                "prev_bigram": word[i - 2:i] if i > 1 else "",
                "next_bigram": word[i + 1:i + 3] if i + 2 < len(word) else "",
                
                # Morphological features
                "prefix": prefix,
                "suffix": suffix,
                "is_first_letter": i == 0,
                "is_last_letter": i == len(word) - 1,
                "is_soft_consonant": is_soft_consonant,
                "case_pattern": 1 if suffix in ["ka", "ce", "ku"] else "",

                # Phonetic rules
                "is_after_soft_consonant": is_soft_consonant,
                
                # Lexical flags
                "is_foreign_word": any((char in "wxq") for char in word),
                "is_capitalized": word[0].isupper(),

            })

    return data


def deconstruct_text(text, letter, target=None):
    """Deconstruct text and create training data."""
    text = clean_text(text)
    words = text.split()
    data = extract_features(words, letter)

    targets = []
    if target is not None:
        target = clean_text(target)
        target_words = target.split()
        for word, target_word in zip(words, target_words):
            for i, l in enumerate(word):
                if l == letter:
                    targets.append(target_labels[target_word[i].lower()])
    return data, np.array(targets)


def create_pipeline(args):
    return Pipeline([
        ("vectorizer", DictVectorizer(sparse=True)),
        ("scale", StandardScaler(with_mean=False, with_std=True)),
        ("classifier", SGDClassifier(
            max_iter=5000,
            loss="log_loss",
            random_state=42,
            verbose=args.verbose))
    ])


class Dataset:
    LETTERS_NODIA = "acdeeinorstuuyz"
    LETTERS_DIA = "áčďéěíňóřšťúůýž"
    DIA_TO_NODIA = str.maketrans(LETTERS_DIA + LETTERS_DIA.upper(),
                                 LETTERS_NODIA + LETTERS_NODIA.upper())

    def __init__(self, name="fiction-train.txt",
                 url="https://ufal.mff.cuni.cz/~straka/courses/npfl129/2223/datasets/"):
        if not os.path.exists(name):
            print(f"Downloading dataset {name}...", file=sys.stderr)
            licence_name = name.replace(".txt", ".LICENSE")
            urllib.request.urlretrieve(url + licence_name, filename=licence_name)
            urllib.request.urlretrieve(url + name, filename=name)

        with open(name, "r", encoding="utf-8-sig") as dataset_file:
            self.target = dataset_file.read()
        self.data = self.target.translate(self.DIA_TO_NODIA)


def main(args):
    
    if args.predict is None and args.text is None:
        # Training phase
        np.random.seed(args.seed)
        train = Dataset()
        print("Training...")

        model = {}
        for letter in diacriticized_letters:
            if args.verbose:
                print(f"Processing letter: {letter}")
            x, y = deconstruct_text(train.data, letter, train.target)
            if len(y) == 0:
                continue
            pipeline = create_pipeline(args)
            model[letter] = pipeline.fit(x, y)

        print("Saving model...")
        with lzma.open(args.model_path, "wb") as model_file:
            pickle.dump(model, model_file)

    else:
        # Prediction phase
        if args.text:
            text = args.text
        else:
            test = Dataset(args.predict)
            text = test.data

        with lzma.open(args.model_path, "rb") as model_file:
            model = pickle.load(model_file)

        new_text = list(text)
        for letter in diacriticized_letters:
            x, _ = deconstruct_text(text, letter)
            if len(x) == 0:
                continue
            predictions = model[letter].predict(x)
            pred_index = 0
            for i, l in enumerate(new_text):
                if l == letter and pred_index < len(predictions):
                    if predictions[pred_index] != 0:
                        new_text[i] = inv_labels[letter][predictions[pred_index]]
                    pred_index += 1

        result = "".join(new_text)
        if args.text:
            print(result)
        return result


if __name__ == "__main__":
    args = parser.parse_args([] if "__file__" not in globals() else None)
    output = main(args)
    if args.save:
        with open("test1.my.txt", "w") as f:
            f.write(output)


