# Tasks 02 (student tomisz)

| Total                                            |    88 |
|--------------------------------------------------|------:|
| 02/tree.py                                       |    30 |
| 02/wildcards.md (Q1, contents)                   |    15 |
| 02/wildcards.md (Q2, wildcard)                   |    13 |
| 02/uptime.txt                                    |    10 |
| 02/architecture.sh                               |    20 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Tasks+02)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## 02/tree.py

✅ **Submitted** (passed, informative only)

✅ **Correct Python script** (passed, +3 points)

✅ **Module-ready** (passed, +2 points)

✅ **Empty directory** (passed, +5 points)

✅ **Files only** (passed, +5 points)

✅ **Only directories** (passed, +5 points)

✅ **Example from web** (passed, +5 points)

✅ **Symlinks** (passed, +5 points)



### General notes (collected from all solutions)

Here are again some general notes about your `tree.py` implementation
as we gathered them across all solutions.

It seems that you are still afraid of functions :-). Do not be. Even for
simple things.

On the other hand, it seems that you are not afraid of global variables.
You should be :-).

The following function your program more readable. Seriously. Even this little
piece of code simplifies reading it. You are saying your intention, not
revealing an implementation detail.

```python
def is_hidden_file(file_name)
    return file_name.startswith(".")
```

The same reasoning applies to names of your identifiers. Names such as
`recursion` or `scan` carry very little meaning. `print_dir_tree` is a huge
improvement over that, requiring very little effort :-).

When joining (pasting) paths, consider using
[`os.path.join`](https://docs.python.org/3/library/os.path.html#os.path.join)
that is smart enough to insert the right slash depending on your OS etc.
It also conveys the message what you are trying to accomplish.

If you know Python well, consider using list comprehension more often:

```python
entries = sorted([
    entry
    for entry in it
    if not is_hidden_file(entry.name)
])
```

It will be a bit faster than appending to the list and it is still quite
readable. Using a separate function `yield`ing the results would be a bit
nicer but it seems that `yield` was not mentioned much during Programming I
course.

By the way, if all branches inside a loop ends with `break`, perhaps the
loop is not needed at all.

```python
def main(path):
	for entry in ... os.scandir(path) ...:
		while True:
			if ...:
				if ...:
					break
				else:
					main(path)
					break
			elif ...
                ...
				break
			else:
				break

```

Last thing: think on how you structure your functions. The following is an
illustration of two approaches. Which one is better?

```python
def main():
    with os.scandir('.') as it:
        for entry in it:
            ...
            print_subtree('', entry.name)

def print_subtree(indent, top):
    with os.scandir(top) as it:
        for entry in it:
            ...
            print_subtree(indent + "    ", top + '/' + entry.ename)

# ----

def main():
    print_tree('.', 0)

def print_tree(path, indent_depth):
    with os.scandir(top) as it:
        for entry in it:
            ...
            print_tree(os.path.join(path, entry.name), indent_depth + 1)
```


## 02/wildcards.md (Q1, contents)

✅ **A1 answered** (passed, informative only)

✅ **A1 completely correct** (passed, +3 points)

✅ **A1 correct** (passed, +4 points)

✅ **A1 contains the right mix of letters** (passed, +8 points)



### General notes (collected from all solutions)

This task was checked automatically but we tried to add less strict tests
to accomodate less precise answers.


## 02/wildcards.md (Q2, wildcard)

✅ **A2 answered** (passed, informative only)

✅ **A2 correct** (passed, +15 points)

❌ **No command in A2** (failed, 2 points subtracted) \

```
Command instead of wildcard detected (cat [0-9][imostz]z.txt).
```

✅ **No comma in A2** (passed, +0 points)

✅ **No quotation mark in A2** (passed, +0 points)



## 02/uptime.txt

**Points**: 10

But what does the number mean?


## 02/architecture.sh

✅ **Submitted** (passed, informative only)

✅ **Works** (passed, +20 points)

❌ **Works in a portable way** (failed, informative only) \

```
-- Command output differs --
output (2 lines):
  x86_64-nswi177
  Warning: option -i is not portable, prefer -m.
expected (1 lines):
  x86_64-nswi177
--
```



### General notes (collected from all solutions)

A short note on so-called _portability_. Several of you decided to
call `arch(1)` or use `uname -i`.

That is fine, this course is about a modern Linux.

But sometimes being a bit more conservative can be useful.

For example, `arch` is not a program that is installed by default, finding
an installation without `uname` would be quite difficult. Hence it makes more
sense to use `uname`.

If we run `uname --help` we would probably see something like this:

```text
Usage: uname [OPTION]...
Print certain system information.  With no OPTION, same as -s.

  -a, --all                print all information, in the following order,
                             except omit -p and -i if unknown:
  -s, --kernel-name        print the kernel name
  -n, --nodename           print the network node hostname
  -r, --kernel-release     print the kernel release
  -v, --kernel-version     print the kernel version
  -m, --machine            print the machine hardware name
  -p, --processor          print the processor type (non-portable)
  -i, --hardware-platform  print the hardware platform (non-portable)
  -o, --operating-system   print the operating system
      --help     display this help and exit
      --version  output version information and exit

GNU coreutils online help: <https://www.gnu.org/software/coreutils/>
Report any translation bugs to <https://translationproject.org/team/>
Full documentation <https://www.gnu.org/software/coreutils/uname>
or available locally via: info '(coreutils) uname invocation'
```

Notice that `-p` and `-i` contains the words _non-portable_. That generally
means that these options may not be available on all installations.

As a matter of fact, there are plenty of so-called Unix-like systems. All
these systems share a common ancestor (in philosophy if not in the source
code) called Unix that started back in 1970s.

Avoiding non-portable extension means that you script would be able to run
on a wider range of systems/platforms. It is of greater interest for system
developers or system administrators rather than normal users.

It is not a big mistake: we target Linux but it is good to know what such
notice about portability means and it is up to you to decide whether you need
it or not.

That is why we have added this as an extra test to draw your attention to it.

Note that we have mocked the original `uname` command for our tests to ensure
that you are actually calling it (hence the custom architecture).


