#!/bin/bash

set -ueo pipefail

for FILE in ${@:-$(ls)}; do
    if ! test -e $FILE; then
        echo "$FILE: no such file or directory."  >&2
    elif test -f $FILE; then
        echo $FILE $(stat $FILE -c %s )
    elif test -d $FILE; then
        echo $FILE "<dir>"
    else
        echo $FILE "<special>"
    fi
done | column --table --table-noheadings --table-columns FILE,SIZE --table-right SIZE