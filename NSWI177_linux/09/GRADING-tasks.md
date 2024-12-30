# Tasks 09 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| 09/passwd.txt                                    |    20 |
| 09/signals.txt                                   |    25 |
| 09/countdown.sh                                  |    35 |
| 09/dnf.txt                                       |    20 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+09)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 09/passwd.txt

✅ **Submitted** (passed, informative only)

✅ **Not executable** (passed, +0 points)

✅ **Correct answer** (passed, +20 points)



### General notes (collected from all solutions)

A small practical note: the easiest way for solving this task was
simply to execute the following command on `linux.ms.mff.cuni.cz`.

```shell
getent passwd $USER >09/passwd.txt
```

It was not really needed to copy anything anywhere, especially if
that meant adding Windows-style line endings to the file :-).



## 09/signals.txt

✅ **Submitted** (passed, informative only)

✅ **Not executable** (passed, +0 points)

✅ **Correct format** (passed, +5 points)

✅ **Correct answer** (passed, +20 points)



## 09/countdown.sh

✅ **Submitted** (passed, informative only)

✅ **Executable** (passed, +0 points)

✅ **Shebang** (passed, +0 points)

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warnings** (passed, +0 points)

✅ **Shellcheck infos** (passed, informative only)

✅ **Shellcheck stylistic** (passed, informative only)

✅ **Countdown from 8** (passed, +8 points)

✅ **Countdown from 2** (passed, +7 points)

✅ **Countdown from 15, Ctrl-C after 4 secs** (passed, +20 points)



## 09/dnf.txt

✅ **Submitted** (passed, informative only)

✅ **Not executable** (passed, +0 points)

✅ **Not version of MSIM itself** (passed, +10 points)

✅ **Correct format** (passed, +5 points)

✅ **Correct answer** (passed, +5 points)



