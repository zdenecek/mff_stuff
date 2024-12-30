#!/bin/bash

set -ueo pipefail
set -x

page="$1"
shift

pandoc \
    --template template.html \
    -B _menu.html "src/${page}.md" \
    "$@" \
    > "public_html/${page}.html"


