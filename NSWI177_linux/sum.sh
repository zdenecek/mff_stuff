#!/bin/bash

pts=$( grep -E -o "Total.*" ./*/GRADING* | sed -r -e "s=.*Total *\|==" -e "s=\|==" | paste -sd "+" - | bc )
files=$( find ./*/GRADING* | wc -w )
max=$(( files * 100 + 100 ))
percent=$(( pts * 100 / max ))
echo "$pts out of $max (${percent}%)"