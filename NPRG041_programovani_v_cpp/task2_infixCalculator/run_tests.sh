#!/bin/bash

RED='\033[0;31m'          # Red
GREEN='\033[0;32m'        # Green
OFF='\033[0m'       # Text Reset

res_di=tests
executable=$1
fail=0
pass=0

for test_in in tests/*.in ; do
    test_without_extension="${test_in%.*}"
    test=$(basename -- "$test_without_extension")

    test_out_expected="$test_without_extension.out"
    test_out_actual="$res_di/$test.my.out"
    test_err="$res_di/$test.my.err"

    # run the executable
    ./$executable < "$test_in" > "$test_out_actual" 2>"$test_err"

    if ! $( cmp --silent "$test_out_actual" "$test_out_expected" ); then
        printf "${RED}failed $test${OFF}\n"
        diff -y --strip-trailing-cr  "$test_out_actual" "$test_out_expected"
        
        fail=1
    else
        printf  "${GREEN}passed $test${OFF}\n"
        pass=$((pass+1))
    fi
done

if [ $fail = 0 ]; then
    echo "all tests passed! ^.^"
else 
    echo "some tests failed :(, but $pass tests passed, you are awesome!"
fi
