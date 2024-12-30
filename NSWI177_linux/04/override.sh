#!/bin/bash

set -ueo pipefail

test -f .NO_HEADER || (cat HEADER 2>/dev/null || (echo "Error: HEADER not found." >&2 && exit 1 ))