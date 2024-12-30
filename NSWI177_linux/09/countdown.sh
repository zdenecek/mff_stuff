#!/bin/bash

set -ueo pipefail

trap "echo Aborted; exit 17" SIGINT TERM

for((NUM=$1;NUM>=1;NUM--)); do
echo "$NUM"
sleep 1
done