#!/bin/bash

set -ueo pipefail

echo "$1" | sed -E ":x; s:[^/(..)]*/\.\./::;  s:/\./:/:; s:^\./::; tx"