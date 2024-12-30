#!/bin/bash

set -ueo pipefail

# Uncomment if you need to debug the generated script
#SPLITTER="tee /dev/stderr"

SPLITTER=${SPLITTER:-cat}


get_column() {
    echo "$1" | tr ',' '\n' | sed -n "${2}p"
}

prepare_row_computation() {
    local index column value
    echo "set -ueo pipefail"
    index=0
    for column in $( echo "$1" | tr ',' ' ' ); do
        index=$(( index + 1 ))
        value="$( get_column "$2" "$index" )"
        echo "$column='$value'"
    done
    # shellcheck disable=SC2016
    echo 'echo $((' "$3" '))'
}

expression="$1"

read -r headers

new_col_name="$( echo "$expression" | cut '-d=' -f 1 )"
new_col_expression="$( echo "$expression" | cut '-d=' -f 2 )"


exit_code=0
echo "$headers,$new_col_name"
while read -r csv_line; do
    col_value="$( \
        prepare_row_computation "$headers" "$csv_line" "$new_col_expression" \
        | $SPLITTER \
        | bash 2>/dev/null \
        || echo '-'
    )"
    if [ "$col_value" = "-" ]; then
        echo "Computation failed at $csv_line." >&2
        exit_code=1
        continue
    fi
    echo "$csv_line,$col_value"
done

exit $exit_code
