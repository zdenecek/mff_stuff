# Before class quiz for 13 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| Q1 (previous lab)                                |    25 |
| Q2 (build systems purpose)                       |    25 |
| Q3 (Pandoc template with login)                  |    25 |
| Q4 (Paw list)                                    |    25 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Before+class+quiz+for+13)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## Q1 (previous lab)

**Points**: 25


## Q2 (build systems purpose)

**Points**: 25

**Expected:** 3 (answered 3)


## Q3 (Pandoc template with login)

✅ **A3 correct** (passed, +25 points)



### General notes (collected from all solutions)

To answer the following question, it was enough to login to
`linux.ms.mff.cuni.cz` via SSH (you had to do that anyway to
get the files) and execute the following.

    pandoc --template template.tpl -V "login=$( whoami )" input.md

Copying the files to your machine and running Pandoc there was
also possible but it required much more work.


## Q4 (Paw list)

**Points**: 25

**Expected:** 3 (answered 3)


### General notes (collected from all solutions)

Again, the best approach was to actually execute the template.

    pandoc --template template.tpl members.md

This rendered the following output.

    A. Marshall (red)
    B. Rubble (yellow)
    C. Chase (blue)
    D. Rocky (green)
    E. Zuma (orange)
    F. Skye (pink)
    G. Everest (purple)
    H. Tracker (olive)

Notice that this does not contain any HTML and the only upcasing
of letters happened for the numbering.

Note that for both Q3 and Q4 it was not really necessary to
understand the template but only execute the command.


