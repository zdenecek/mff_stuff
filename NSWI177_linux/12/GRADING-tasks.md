# Tasks 12 (student tomisz)

| Total                                            |    40 |
|--------------------------------------------------|------:|
| 12/csv_calc.sh                                   |    10 |
| 12/UPSTREAM.md                                   |    30 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+12)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 12/csv_calc.sh

✅ **Issue for code injection exists with correct marker** (passed, +10 points)

✅ **Issue for code injection has non-empty title** (passed, +10 points)

❌ **Issue for code injection has reasonable description** (failed, worth 5 points) \

```
-- Issue description is insufficient. --
iid      : 2
title    : mazání precious.txt [task-malicious]
descr    :
--
```

❌ **Issue for code injection contains some code example** (failed, worth 5 points) \

```
-- Issue descriptions seems to contain no proper code example --
iid      : 2
title    : mazání precious.txt [task-malicious]
descr    :
--
```

❌ **Branch related to the issue exists with a correct name** (failed, worth 10 points) \

```
-- No branch issue/2 found --
Found following branches:
 - origin/HEAD
 - origin/master
--
```

❌ **Possible branch related to the issue found** (failed, informative only) \

```
-- No branch issue/2 found --
Found following branches:
 - origin/HEAD
 - origin/master
--
```

↷ **Branch closes the issue** (skipped)

↷ **Malicious data injection fixed in issue branch** (skipped)

↷ **Malicious data injection fixed as a separate commit** (skipped)

✅ **Base version imported** (passed, +10 points)

✅ **Exit code issue fixed - bad expression** (passed, +10 points)

❌ **Code injection in expression fixed** (failed, worth 10 points) \

```
-- No expression-injection resistant version of csv_calc.sh found --
Found following commits, but none of them provides
a csv_calc.sh fix injection-in-expression fixed.
 - e197b64404ded64b63908faea6d14cc10bad6602 add 12 template
--
```

❌ **Executable** (failed, 30 points subtracted) \

```
-- file is not executable --
path : 12/csv_calc.sh
hint : run chmod +x 12/csv_calc.sh
--
```

✅ **Shellcheck errors** (passed, +0 points)

✅ **Shellcheck warning** (passed, +0 points)



## 12/UPSTREAM.md

✅ **Exists** (passed, informative only)

✅ **Properly merged** (passed, +30 points)



