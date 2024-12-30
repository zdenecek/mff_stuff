#!/bin/bash

set -ueo pipefail

ALL_OK=true
for TEST in "$@"; do
    # Hack to run the binary
    if ./$TEST | ./tools/check_output.py; then
        echo "$TEST OK"
    else
        echo "$TEST FAILED"
        ALL_OK=false
    fi
done

if $ALL_OK; then
    echo "All tests passed."
    exit 0
else
    exit 1
fi
