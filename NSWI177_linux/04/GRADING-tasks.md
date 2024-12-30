# Tasks 04 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| 04/override.sh                                   |    30 |
| 04/second_highest_uid.sh                         |    30 |
| 04/row_sum.sh                                    |    40 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+04)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 04/override.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Is purely shell script** (passed, +0 points)

✅ **Uses pipes and composition only** (passed, +0 points)

✅ **Normal invocation** (passed, +10 points)

✅ **Suppressed print** (passed, +7 points)

✅ **Suppressed print withour HEADER** (passed, +6 points)

✅ **No files** (passed, +7 points)



### General notes (collected from all solutions)

Here are some general comments gathered over all submitted solutions. Most of
them are not pointing to mistakes at all, but they are just recommendations
what to use in shell scripts or which constructions are typically used.


Several solutions used the following:

```
echo "Error..." >&2 | exit 1
```

While it was working in this case and allowed us to connect `echo` and `exit`
more tightly than with `&&`, we need to know that the script might not be
terminated by exit command.

All commands in pipeline are executed in subshell, so `exit` command only
determined the exit code of the pipeline. Then the script was terminated either
because of failed pipeline if `set -e` was used, or because of reaching the end
of script file, in which case it returned exit code of the last command --
accidentally the pipeline.


Some of you modified the operator precedence by using `{ ...; }` or `(...)`,
which hadn't been revealed yet at time of task publishing. Both of them may be
used for grouping several commands and often even interchangeably, but there are
two important differences. The curly brackets doesn't involve subshell
execution, so you can modify variables inside, change directory, etc.; while
`(...)` are always executed in subshell. From the syntax point of view `(`, `)`
are handled more like language construct not requiring spaces around them; `{`
and `}` are, on the other hand, handled more like commands, so you need to use
spaces around them and you need to properly separate `}` from the previous
command by `;` or newline character. After the following line, `}` is still
expected and, if found, `echo` will print `}`.

```
{ echo }
```

Several solutions used `cat` or `ls` only to determine whether specific file
exists. Although `cat "$file" >/dev/null 2>/dev/null` may behave that way (under
some circumstances), it is much better to use `test -e "$file"`, which tests
just the existence of the file without reading it (read permission is thus not
needed).

Almost everywhere you then will see `[ ... ]` instead of `test ...`; it may
improve readability a little after you get used to it. Just keep in mind that
`]` should be the last argument of `[` command, requiring some spacing.

Similarly `>&2` is more common than `>/dev/stderr`, and 1 is usually omitted in
`1>&2`.


Instead of calling `set -o pipefail; set -e; set -u`, all arguments may be
passed to one call `set -o pipefail -e -u`. Also as is usual for Unix-style
commands, you can merge short options together: `set -euo pipefail`. Note that
the `-o` option must be the last one, because argument is expected after it.


To enhance readability, it is allowed to break line immediately after `||`,
`&&`, or `|`. This is useful mainly when using long pipelines:

```
tr -dc A-Z < /dev/urandom |
  fold -w5 |
  head -n 195 |
  sort |
  paste -sd ' ' |
  fold -s
```


## 04/second_highest_uid.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Is purely shell script** (passed, +0 points)

✅ **Uses pipes and composition only** (passed, +0 points)

✅ **Input one** (passed, +10 points)

✅ **Input two** (passed, +10 points)

✅ **Input three** (passed, +10 points)



### General notes (collected from all solutions)

Solutions of this assignment were overall correct,
but there were some unnecessary constructions used.

Some of them were redirecting stdin from stdin or stdout into stdout, or calling
`cat` to read its input and pass it further; such operations do really nothing:

`</dev/stdin ...`, `... <&0`, `... >&1`, `cat /dev/stdin | ...`

We also don't need to pass `/dev/stdin` as the only input file of `cut` or
`cat`, as they (and many other commands) use stdin automatically if no file is
provided. If we really need to specify it explicitly, we can usually use `-`
instead: `cat a.txt - b.txt`


Similarly there is no need for storing the whole input into variable, if we
need to process the input only once:

```
input=$(cat /dev/stdin)
echo "$input" | ...
```

The last and possibly dangerous construction is the following:

`cat $1 | ...`

It looks like a simple way to allow user passing its input either on stdin
or as file name, but it will not work when the file name contains spaces.
In such a case, the name will be broken into several arguments and `cat`
(or other command) will work with different files.
The safe way how to do this is passing all arguments as they are:

`cat "$@" | ...`

This way even zero number of arguments will work properly, in contrast to `cat "$1"`.


## 04/row_sum.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Is purely shell script** (passed, +0 points)

✅ **Uses pipes and composition only** (passed, +0 points)

✅ **Big matrix** (passed, +10 points)

✅ **One row matrix** (passed, +10 points)

✅ **One column matrix** (passed, +10 points)

✅ **One element matrix** (passed, +10 points)



### General notes (collected from all solutions)

Some solutions used `cut` to remove beginnings and together with `rev` even
ends of lines. But there were also some void filters used, as `cut` counts
characters and fields from 1: `... | cut -c 1- | ...`
This simply passes always the whole line from its first character to the end.


Several solutions used `bc | cat` or something similar. This may seem useful
when using `bc` interactively, because it suppresses its welcome message if
either stdin or stdout is not connected to terminal. On the other hand, if we
feed bc with pipe from other command, the precondition is already satisfied and
so using pipe even on the output side is not necessary. The other way, how to
get `bc` suppress its welcome message, is via `-q` option.


Several solutions used `tr` multiple times:

```
tr -s ' ' | tr ' ' '+' | tr '|' '0'
```

This can be used to squeeze each sequence of spaces into just one space,
replace it with plus and replace bordering '|' with zeroes bypassing any need
for cutting the sides. But it can be even simpler, the following is equivalent
as there are never multiple `|` next to each other:

```
tr -s '| ' '0+'
```


