#!/bin/bash

set -ueo pipefail

function print_help {
echo Usage: sysinfo [options]
echo " -c   --cpu     Print number of CPUs."
echo " -l   --load    Print current load."
echo " -k   --kernel  Print kernel version."
echo " -s   --script  Each value on separate line."
echo
echo "Without arguments behave as with -c -l -k."
echo
echo "Copyright NSWI177 2022"
}

opts_short="lkcsh"
opts_long="load,kernel,cpus,help,script"

# Check for bad usage first (notice the ||)
getopt -Q -o "$opts_short" -l "$opts_long" -- "$@" || exit 1

# Actually parse them (we are here only if they are correct)
eval set -- "$( getopt -o "$opts_short" -l "$opts_long" -- "$@" )"

DELIMITER=" "
LOAD=false
KERNEL=false
CPUS=false
ANY=false

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            print_help
            exit 0
            ;;
        -s|--script)
            DELIMITER="\n"
            ;;
        -l|--load)
            LOAD=true
            ANY=true
            ;;
        -c|--cpus)
            CPUS=true
            ANY=true
            ;;
        -k|--kernel)
            KERNEL=true
            ANY=true
            ;;

        --)
            shift
            break
            ;;

        *)
            echo "Unknown option $1" >&2
            exit 1
            ;;
    esac
    shift
done

(if ! $ANY || $LOAD; then
    echo -n "load="
    cut -d " " -f 1 /proc/loadavg 
fi
if ! $ANY || $KERNEL; then
    echo -n "kernel="
    uname -r
fi
if ! $ANY || $CPUS; then
    echo -n "cpus="
    nproc
fi) | paste -sd"$DELIMITER"

