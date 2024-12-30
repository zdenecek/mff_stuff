# Before class quiz for 11 (student tomisz)

| Total                                            |    97 |
|--------------------------------------------------|------:|
| Q1 (previous lab)                                |    25 |
| Q2 (virtual environments)                        |    25 |
| Q3 (venv is not commited)                        |    22 |
| Q4 (semantic versioning)                         |    25 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Before+class+quiz+for+11)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## Q1 (previous lab)

**Points**: 25


## Q2 (virtual environments)

**Points**: 25

**Expected:** 4 (answered 4)


### General notes (collected from all solutions)

Brief explanation why we think 4 is the best anwer.

Option 1 is mixes cloning and virtual environment while they are orthogonal
concepts.

Option 2 is correct but it is actually a subset of option 4 which talks
about isolated environment.

Option 3 is kind of correct but testing on different systems does not require
virtual environments (i.e. compilation and testing can be performed without
virtual environments).

Option 4 basically summarizes the motivation from the whole before-class
reading text.

Option 5: packages from PyPI can be installed using the same tools within
or outside virtual environments without a difference (except that outside
the environment `pip` will -- by default -- install system wide and will
require superuser privileges).


## Q3 (venv is not commited)

**Points**: 22

**Expected:** 1, 2, 4 (answered 1, 2, 3)


### General notes (collected from all solutions)

We have realized that some of the options were probably not covered in
enough detail in the before-class reading text. For these we have not
subtracted any points even in case of wrong answer.

Note that option 2 is something that was already mentioned several times.

Option 4 was probably not clear from the text as we talked about OS-specific
files only. If you have looked inside the `venv` directories during labs,
you probably noticed the absolute paths there (we accepted bad answer here).

Option 5 was probably a bit misleading but the truth is that Git can version
large number of files quite wel (we accepted bad answer here).



## Q4 (semantic versioning)

**Points**: 25

**Expected:** 1, 5, 9 (answered 1, 5, 9, 12)


### General notes (collected from all solutions)

Few notes for the semantic versioning.

Removing a function is not a backward-compatible change, hence major version
upgrade is needed.

Adding a function is a backwards-compatible change for most languages and
environments, hence it is about minor version changes (but we accepted
changes to major version as well in this case).

Fixing a bug requires changes for the patch version (no need to update
the minor version as the API was not modified).

Changing the implementation does not require any change in the versions
as we have not modified API nor functionality (in the sense of black box).
But we have allowed option 12 as correct as there might be some confusion
between version and revisions (commits).

In this sense, semantic versioning does not care about commits but about
publishable artifacts. Therefore, you can create hundreds of commmits
before updating the patch version because you were only refactoring your
code. There might be thousands of merge requests before updating the major
version as long as you are adding new functions or fixing bugs.


