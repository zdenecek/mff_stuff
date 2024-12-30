#!/bin/bash

set -ueo pipefail

tr -d "|" | tr -s " " | sed 's/^[ \t]*//;s/[ \t]*$//' | tr " " + | bc