#!/bin/bash

set -ueo pipefail

find "$@" -type f -name '*.[ch]' -print0 | (
    error_count=0
    while IFS= read -r -d $'\0' filename; do
        if ! clang-format -style=file "$filename" | diff -ud - "$filename"; then
            error_count=$(( error_count + 1 ))
        fi
    done
    exit "$error_count"
)
exit "$?"
