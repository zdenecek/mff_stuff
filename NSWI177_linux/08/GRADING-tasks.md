# Tasks 08 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| 08/timeconv.sh                                   |    20 |
| 08/ip.sh                                         |    20 |
| 08/normalize.sh                                  |    20 |
| 08/markdown.sh                                   |    40 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+08)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 08/timeconv.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Example run** (passed, +7 points)

✅ **All AM times** (passed, +5 points)

✅ **All PM times** (passed, +5 points)

✅ **Ignored entries** (passed, +3 points)

✅ **No modifications to files in CWD** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



### General notes (collected from all solutions)

We greatly appreciate that several of you actually tried to build
the whole script in `sed`. The solutions that we have seen were however
incomplete and unfortunately did not work in all cases.

Otherwise, it makes perfect sense to build the script like this:

```
#!/bin/sed -f

s/12:\([0-5][0-9]\)PM/12:\1/g
...
```

Try to avoid needless memory consumption. We have seen the following
snippet too many times to leave it without comment.

```shell
input="$( cat "$input_file" )"
echo "$input" | sed ...
```

Imagine a 4GB file where each line is less than 1KB long. The above script
would read the whole file into memory into `$input` variable. Calling `echo`
could easily lead to allocation to another 4GB for the parameter inside
`echo` ;-). Actually, there is a rather small limit on the lenght of the
command line (about 2MB) so it would not even work for some inputs.

Compare to simple `sed ... < "$input_file"` where the file would be read
line by line only. Assuming the above mentioned line length, `sed` would
require few kilobytes for building the regular expression and one KB for the
line.

We always execute your tests on small data so it seems this does not matter.
But it does. Shell utilities and pipes are practically useful for filtering
large data and preprocessing them before loading them to more specialized
tools. Get into habit of thinking about the performance a little bit.


## 08/ip.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Single entry** (passed, +6 points)

✅ **Multiple entries I** (passed, +7 points)

✅ **Multiple entries II** (passed, +7 points)

✅ **No modifications to files in CWD** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



### General notes (collected from all solutions)

Iterating over individual lines is often not needed at all in shell
scripts.

The following snippet is a typical example of completely useless while
loop that only complicates reading and degrades performance (while we
generally do not care much about shell script performance, the difference
between running `sed` once or starting it thousand times could have
serious impact).

```shell
while read -r line; do
    result="$( echo "$line" | sed -e '...' )"
    result="$( echo "$result" | cut ... )"
    echo "$result"
done <"$input"
```

Because the above is completely equivalent to the following pipe ;-)

```shell
<"$input" sed -e '...' | cut ...
```

Many of you decided to use some kind of temporary files to store counters
for each IP address. Funny thing is that with proper filtering, the counting
can be done on the fly quite easily:

```shell
grep 404 # This shall be replaced by matching at the exact position but it
         # is enough to get the right lines
| cut -d ' ' -f 1 # Get the IP address only
# At this point, we are solving a problem of finding the most frequent line
# Hence something along the lines of
| sort | uniq -c | sort -n | tail -n 1
```

No temporary files and actually the whole script can be expressed in two
functions:

```shell
get_malicious_ip_addresses | find_most_frequent_line
```

And what is most important: these two functions can be tested (or debugged)
separately :-)


## 08/normalize.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Example run I** (passed, +3 points)

✅ **Example run II** (passed, +3 points)

✅ **Example run III** (passed, +3 points)

✅ **Example run IV** (passed, +3 points)

✅ **Lots of parent directories** (passed, +3 points)

✅ **Special characters** (passed, +3 points)

✅ **Complex one** (passed, +2 points)

✅ **No modifications to files in CWD** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



### General notes (collected from all solutions)

The most common issue here was handling the repeated replacements.
If the conditional jumps in `sed` seemed frightening to you, simple while
loop can solve it too :-)

```shell
path="$1"

while true; do
    cleaned="$( echo "$path" | sed ... )"
    if [ "$cleaned" = "$path" ]; then
        break
    fi
    path="$cleaned"
done
```

Do not forget that `sed` has the `g` switch for substitutions, thus
removal of `./` can be done in a single run: `s:/[.]/:/:g`.



## 08/markdown.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Emphasis** (passed, +5 points)

✅ **Strong emphasis** (passed, +5 points)

✅ **Entities** (passed, +4 points)

✅ **Simple link** (passed, +5 points)

✅ **Multiple emphasis** (passed, +3 points)

✅ **Multiple lines** (passed, +3 points)

✅ **Link with query** (passed, +5 points)

✅ **Artificial link** (passed, +5 points)

✅ **Many links** (passed, +5 points)

✅ **No modifications to files in CWD** (passed, informative only)

✅ **Temporary files properly removed** (passed, informative only)



