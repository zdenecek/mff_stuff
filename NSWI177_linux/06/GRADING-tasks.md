# Tasks 06 (student tomisz)

| Total                                            |   200 |
|--------------------------------------------------|------:|
| 06/dir.sh                                        |    30 |
| 06/scoring.sh                                    |    50 |
| 06/sysinfo.sh                                    |    50 |
| 06/plot.sh                                       |    70 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+06)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 06/dir.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Example invocation without formatting** (passed, +6 points)

✅ **Example invocation** (passed, +4 points)

✅ **In current directory without formatting** (passed, +3 points)

✅ **In current directory** (passed, +2 points)

✅ **Special only without formatting** (passed, +3 points)

✅ **Special only** (passed, +2 points)

✅ **Inside empty directory** (passed, +5 points)

✅ **Non-existent file without formatting** (passed, +3 points)

✅ **Non-existent file** (passed, +2 points)

✅ **No existing files in CWD destroyed** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



### General notes (collected from all solutions)

We have seen a lot of duplicate code for two situations: user provides
list of files and user provides no arguments.

Notice how in our solution we bypass this problem by encapsulating it in
a function.

Also note how loop output can be piped for further formatting.

```shell
#!/bin/bash

set -ueo pipefail

show_it() {
    local filename
    local size
    
    for filename in "$@"; do
        if [ -d "$filename" ]; then
            size="<dir>"
        elif [ -f "$filename" ]; then
            size="$( stat --format='%s' "$filename" )"
        elif [ -e "$filename" ]; then
            size="<special>"
        else
            echo "$filename: no such file or directory." >&2
            continue
        fi
        echo "$filename $size"
    done | column --table --table-noheadings --table-columns FILENAME,SIZE --table-right SIZE
}

if [ $# -eq 0 ]; then
    if [ -z "$( ls -A )" ]; then
        exit 0
    fi
    show_it *
else
    show_it "$@"
fi
```


## 06/scoring.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Example run** (passed, +20 points)

✅ **Funny names** (passed, +10 points)

✅ **Many teams** (passed, +10 points)

✅ **No data** (passed, +5 points)

✅ **No existing files in CWD destroyed** (passed, +2 points)

✅ **Temporary files properly removed** (passed, +3 points)



### General notes (collected from all solutions)

Your solutions generally surprised us by their complexity. This task
was supposed to be the easiest to complete :-).

Below is our solution. Notice that we use `mktemp` to create temporary
directory. **Always** create directories like this: you can never now what
files are in the current directory and it is pretty surprising if some file
is overwritten only because it was named `t.txt`.

Our extended test suite tests for that as well in all assignments for 06 so
feel free to check the results of the last two tests to see how your script
behaved.

Kind reader will add proper destruction of the temporary directory `$datas`
into this solution.

Notice that the score is summed at the end of the script: there is really
no need to recompute it all the time and `paste ... | bc` works very well
here.

```shell
#!/bin/bash

set -ueo pipefail

datas="$( mktemp -d )"

while read -r team score; do
    echo "$score" >>"$datas/$team"
done

if [ -z "$( ls -A "$datas" )" ]; then
    exit 0
fi

cd "$datas"
for team in *; do
    echo "${team}:$( paste -s -d + "$team" | bc )"
done
```


## 06/sysinfo.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Default run** (passed, +20 points)

✅ **Example run I** (passed, +8 points)

✅ **Example run II** (passed, +8 points)

✅ **Example run III** (passed, +8 points)

✅ **Run with --help** (passed, +6 points)

✅ **No existing files in CWD destroyed** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



### General notes (collected from all solutions)

We have noticed a lot of duplicate code. Mostly it was caused by
re-parsing the options several times or using different code for printing
everything in one-line versus printing with `--script`.

The following solution tries to clearly separate program configuration
(command-line parsing) and main program execution.

Notice how information about what to print are stored and how is the
final output formatted.

```shell
#!/bin/bash

set -ueo pipefail

output_merged() {
    paste -s -d ' '
}

show_help() {
cat <<'EOF_HELP'
Usage: sysinfo [options]
 -c   --cpu     Print number of CPUs.
 -l   --load    Print current load.
 -k   --kernel  Print kernel version.
 -s   --script  Each value on separate line.

Without arguments behave as with -c -l -k.

Copyright NSWI177 2022
EOF_HELP

}

opts_short="lkcsh"
opts_long="load,kernel,cpus,help,script"

getopt -Q -o "$opts_short" -l "$opts_long" -- "$@" || exit 1
eval set -- "$( getopt -o "$opts_short" -l "$opts_long" -- "$@" )"

print_all=true
print_load=false
print_kernel=false
print_cpus=false
output_formatter=output_merged

for i in "$@"; do
    case "$i" in
        -l|--load)
            print_load=true
            print_all=false
            ;;
        -k|--kernel)
            print_kernel=true
            print_all=false
            ;;
        -c|--cpus)
            print_cpus=true
            print_all=false
            ;;
        -s|--script)
            output_formatter="cat"
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        --)
            ;;    
        *)
            show_help
            exit 1
            ;;
    esac
done

if $print_all; then
    print_load=true
    print_kernel=true
    print_cpus=true
fi

(
    if $print_load; then
        echo "load=$( cut -d ' ' -f 1 /proc/loadavg )"
    fi
    if $print_kernel; then
        echo "kernel=$( uname -r )"
    fi
    if $print_cpus; then
        echo "cpus=$( nproc )"
    fi
) | $output_formatter
```


## 06/plot.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Example run I** (passed, +15 points)

✅ **Example run II** (passed, +10 points)

✅ **Example run III** (passed, +15 points)

✅ **Same values** (passed, +10 points)

✅ **COLUMNS not set** (passed, +10 points)

✅ **No data** (passed, +10 points)

✅ **No existing files in CWD destroyed** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



