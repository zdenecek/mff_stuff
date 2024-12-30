#!/bin/bash

set -ueo pipefail


echo '* [Homepage](index.html)'
for course_meta in "$@"; do
    name="$( jq -r .title <"${course_meta}" )"
    code="$( jq -r .code <"${course_meta}" )"
    echo "* [${name} (${code})](${code}.html)"
done | sort
