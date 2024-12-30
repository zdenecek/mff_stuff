#!/bin/bash

set -ueo pipefail

# 164.66.80.295 - - [25/Mar/2022:07:07:22 +0100] "GET /teaching/nswi177/labs/07/ HTTP/1.1" 200

grep -E "([0-9.]) - - \[.*\] \"(GET|POST) .*\" 404"  | sed -E "s/([0-9.]*) - -.*/\1/" | sort | uniq -c | sort -n | tail -n 1 | sed -E "s/ *[0-9]* (.*)/\1/"