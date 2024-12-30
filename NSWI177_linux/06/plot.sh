#!/bin/bash

set -ueo pipefail

COLUMNS=${COLUMNS:-80}

FILENAME=${1:-test.txt}

MAXLEN=0
MAXVAL=0

while read VALUE LABEL; do
    (( VALUE > MAXVAL )) && MAXVAL=$VALUE 
    LEN=$(( ${#LABEL} + ${#VALUE} ))
    (( LEN > MAXLEN )) && MAXLEN=$LEN 

done <$FILENAME

# two parentheses and at least two spaces
MAXLEN=$(( MAXLEN + 4))

# 2 for | and a space
PLOTSIZE=$(( COLUMNS - MAXLEN - 2))



while read VALUE LABEL; do
    if [ "$LABEL" == "" ]; then
        continue
    fi

    printf "%-${MAXLEN}s" "$LABEL ($VALUE)"
    echo -n "| "
    COUNT=$(echo "scale=0; ($PLOTSIZE * $VALUE) / $MAXVAL" | bc -l)
    for i in $(seq $COUNT); do echo -n "#";done
    echo
done <$FILENAME
