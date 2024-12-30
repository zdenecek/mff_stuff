# Before class quiz for 08 (student tomisz)

| Total                                            |    87 |
|--------------------------------------------------|------:|
| Q1 (previous lab)                                |    25 |
| Q2 (letters regex)                               |    22 |
| Q3 (numbers regex)                               |    15 |
| Q4 (HTTP regex)                                  |    25 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Before+class+quiz+for+08)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## Q1 (previous lab)

**Points**: 25


## Q2 (letters regex)

**Points**: 22

**Expected:** 1, 3, 5, 6, 9 (answered 1, 3, 4, 5, 6, 9)


## Q3 (numbers regex)

**Points**: 15

**Expected:** 4 (answered 5)


### General notes (collected from all solutions)

The regular expression was not surrounded by any kind of anchors
(e.g. `[a-z][0-9]{4,6}` or `^[0-9]{4,6}`) thus the upper limit
is actually irrelevant in the option with the upper limit.

Note that you could have used `grep` and `sed` to practically
check which of the options make sense and how they work.


## Q4 (HTTP regex)

**Points**: 25

**Expected:** 4, 6 (answered 4, 6)


