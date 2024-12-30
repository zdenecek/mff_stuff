#!/bin/bash

code=0

FILES="$( find . -maxdepth 5 -name \*.sh -type f  )"

for f in $FILES; do
    shellcheck "$f" || code=1
done;

exit "$code"
