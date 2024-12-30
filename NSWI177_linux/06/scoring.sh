#!/bin/bash

set -ueo pipefail


FOLDER="$(mktemp -d)"


while read NAME POINTS; do
    FILENAME="$FOLDER/$NAME"
    touch $FILENAME
    echo $POINTS >>$FILENAME
done 

for NAME in $(ls $FOLDER); do
    FILENAME="$FOLDER/$NAME"
    
    SUM="$(paste -sd+ $FILENAME | bc)"
    echo "$NAME:$SUM"
done


rm -rf $FOLDER