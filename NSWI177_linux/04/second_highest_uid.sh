#!/bin/bash

set -ueo pipefail

cut -f 3 '-d:' | sort -nr | head -n 2 | tail -n 1