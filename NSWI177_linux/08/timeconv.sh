#!/bin/bash

set -ueo pipefail

# The event starts at 03:25PM and is expected to end at 06:17PM.


sed -E "s/(10|11|0[0-9]):([0-5][0-9])AM/\1:\2/g;s/(12):([0-5][0-9])AM/00:\2/g;s/01:([0-5][0-9])PM/13:\1/g;s/02:([0-5][0-9])PM/14:\1/g;s/03:([0-5][0-9])PM/15:\1/g;s/04:([0-5][0-9])PM/16:\1/g;s/05:([0-5][0-9])PM/17:\1/g;s/06:([0-5][0-9])PM/18:\1/g;s/07:([0-5][0-9])PM/19:\1/g;s/08:([0-5][0-9])PM/20:\1/g;s/09:([0-5][0-9])PM/21:\1/g;s/10:([0-5][0-9])PM/22:\1/g;s/11:([0-5][0-9])PM/23:\1/g;s/12:([0-5][0-9])PM/12:\1/g" 

