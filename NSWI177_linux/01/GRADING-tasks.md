# Tasks 01 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| 01/factor.py                                     |    50 |
| 01/Issue                                         |    35 |
| 01/Issue (selected environment)                  |    15 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+01)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 01/factor.py

✅ **Submitted** (passed, informative only)

✅ **Correct Python script** (passed, +3 points)

✅ **Module-ready** (passed, +2 points)

✅ **input.txt not committed** (passed, +0 points)

✅ **Run with 0** (passed, +5 points)

✅ **Run with 1** (passed, +5 points)

✅ **Run with 2** (passed, +5 points)

✅ **Run with 3** (passed, +5 points)

✅ **Run with 4** (passed, +5 points)

✅ **Run with 24** (passed, +5 points)

✅ **Run with 2022** (passed, +5 points)

✅ **Run with -17** (passed, +5 points)

✅ **Run with xy** (passed, +5 points)



### General notes (collected from all solutions)

Here are some general notes about your `factor.py` implementation.
Take them mostly as things to think about: we try to provide them as useful
guidelines that are applicable to programming in general.

First of all, use functions to structure your code and separate individual
parts. The task has three big parts: reading the input, computing the factors
and printing the result (and error handling). Hence, the code should look
like this:

```python
def main():
    try:
        number = get_input()
        for i in get_factors(number):
            print(i)
    except ValueError: # Or your own type
        print('-')
```

The above clearly separates the concerns and nicely captures the intent
of the program.

Related to the above is that Python community generally believes that it is
[easier to ask for forgiveness than permission](https://docs.python.org/3.6/glossary.html#term-eafp).
Do not try checking that the input looks like a number: try to convert it
and react to the exception. That also nicely skips whitespace. Please, never
force a user to remove a whitespace if you can do it too.

Every opened file (more generally, any resource: this includes network socket
or a database cursor) should be eventually closed. Do not rely on your
operating system or Python interpreter to do that for you. Better yet, use
`with` to clearly mark where the file is used:

```python
with open('input.txt', 'r') as f:
    number = f.read()
```

Please, do not overoptimize your code: if you start by searching for all
primes smaller than the input, it will be much much slower than simply trying
the numbers directly. And the code is much worse to maintain too.

Try to avoid use of global variables when they are not needed (fact: they were
not needed for this program). Using variable named `input` is possible but
you actually overwrote an existing function. Probably not something you want
to do.


## 01/Issue

✅ **Correct issue name** (passed, +5 points)

✅ **Confidential issue** (passed, +5 points)

✅ **Assigned to a teacher** (passed, +1 points)

✅ **Assigned to horkv6am** (passed, +4 points)

✅ **/etc/os-release link** (passed, +2 points)

✅ **/etc/os-release link name correct** (passed, +3 points)

✅ **/etc/os-release link correct** (passed, +5 points)

✅ **/proc/loadavg provided** (passed, +5 points)

✅ **/proc/loadavg in correct format** (passed, +5 points)



## 01/Issue (selected environment)

**Points**: 15


### General notes (collected from all solutions)

To clarify things: we were actually interested in which graphical environment
you have selected (but we have accepted also reasoning about your distribution
of choice). We have subtracted points for answers that have not provided any
reason for the choice or were completely off-topic.

To clarify the terms a bit more: distribution is the packaging of how you
get Linux and other programs. They mostly differ in the way how software is
installed and updated. If you have the USB or ISO from us, you have Fedora.

The graphical environment selection (we will for now ignore the differences
between a desktop environment and window manager) is orthogonal to the
distribution. You will find most environments on most distributions.

For the computers in IMPAKT, the distribution is Fedora and there are about
20 environments to choose from (KDE/Plasma, GNOME, Xfce, Openbox, i3 etc.).


