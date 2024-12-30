#!/usr/bin/env python3
import argparse
import lzma
import os
import pickle
import sys
from typing import Optional
import urllib.request

import numpy as np

parser = argparse.ArgumentParser()
# These arguments will be set appropriately by ReCodEx, even if you change them.
parser.add_argument("--predict", default=None, type=str, help="Path to the dataset to predict")
parser.add_argument("--recodex", default=False, action="store_true", help="Running in ReCodEx")
parser.add_argument("--seed", default=42, type=int, help="Random seed")
# For these and any other arguments you add, ReCodEx will keep your default value.
parser.add_argument("--model_path", default="diacritization.model", type=str, help="Model path")


from diacritization_dictionary import Dictionary

class Dataset:
    LETTERS_NODIA = "acdeeinorsuuyz"
    LETTERS_DIA = "áčďéěíňóřšúůýž"

    DIA_TO_NODIA = str.maketrans(LETTERS_DIA + LETTERS_DIA.upper(), LETTERS_NODIA + LETTERS_NODIA.upper())

    def __init__(self,
                 name="fiction-train.txt",
                 url="https://ufal.mff.cuni.cz/~courses/npfl129/2425/datasets/"):
        if not os.path.exists(name):
            print("Downloading dataset {}...".format(name), file=sys.stderr)
            licence_name = name.replace(".txt", ".LICENSE")
            urllib.request.urlretrieve(url + licence_name, filename=licence_name)
            urllib.request.urlretrieve(url + name, filename="{}.tmp".format(name))
            os.rename("{}.tmp".format(name), name)

        with open(name, "r", encoding="utf-8-sig") as dataset_file:
            self.target = dataset_file.read()
        self.data = self.target.translate(self.DIA_TO_NODIA)

def main(args: argparse.Namespace) -> Optional[str]:
    if args.predict is None:
        np.random.seed(args.seed)
        train = Dataset()
        dictionary = Dictionary()

        word_frequencies = {}

        for word, diacritized_word in zip(train.data.split(), train.target.split()):
            if word not in word_frequencies:
                word_frequencies[word] = {}
            if diacritized_word not in word_frequencies[word]:
                word_frequencies[word][diacritized_word] = 0
            word_frequencies[word][diacritized_word] += 1

        with lzma.open(args.model_path, "wb") as model_file:
            pickle.dump((word_frequencies, dictionary.variants), model_file)

    else:
        test = Dataset(args.predict)

        with lzma.open(args.model_path, "rb") as model_file:
            word_frequencies, dictionary_variants = pickle.load(model_file)

        predictions = []
        for word in test.data.split():
            if word in dictionary_variants:
                diacritized_options = dictionary_variants[word]
                if word in word_frequencies:
                    diacritized_options.sort(key=lambda w: -word_frequencies[word].get(w, 0))
                predictions.append(diacritized_options[0])
            elif word in word_frequencies:
                # Select the most frequent diacritized variant from training data
                diacritized_options = list(word_frequencies[word].keys())
                diacritized_options.sort(key=lambda w: -word_frequencies[word].get(w, 0))
                predictions.append(diacritized_options[0])
            else:
                predictions.append(word)

        return " ".join(predictions)

if __name__ == "__main__":
    main_args = parser.parse_args([] if "__file__" not in globals() else None)
    print(main(main_args))