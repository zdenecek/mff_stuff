#!/bin/bash

set -ueo pipefail

EXIT_CODE=0

# We assume C sources have sane naming
find -type f -name '*.[ch]' | (
    while read FILENAME; do
        if ! clang-format --style=file "$FILENAME" | diff -ud -p --color=auto "$FILENAME" -; then
            EXIT_CODE=1
        fi
    done
    exit "$EXIT_CODE"
)

exit "$?"
