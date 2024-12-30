# Before class quiz for 09 (student tomisz)

| Total                                            |   100 |
|--------------------------------------------------|------:|
| Q1 (previous lab)                                |    25 |
| Q2 (setuid vs sudo)                              |    25 |
| Q3 (packages)                                    |    25 |
| Q4 (signals and processes)                       |    25 |

If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi177/2022/common/forum/-/issues/new?issue[confidential]=true&issue[title]=Grading+Before+class+quiz+for+09)
in the _Forum_.


For assignments with automated tests you will see a TAP-style result output
that you are familiar with from your pipeline tests in GitLab.

The tests also contains information about points assigned (or subtracted)
for that particular test. There are also tests with _zero points_ that
are informative only (kind of like warnings from your compiler: you
should pay attention but they are not crucial).

## Q1 (previous lab)

**Points**: 25


## Q2 (setuid vs sudo)

**Points**: 25


### General notes (collected from all solutions)

Here is an overview of what we wanted to hear (more or less).

Both `passwd` and `dnf` modify some kind of system database: either user
passwords or list of installed packages. So both need to run with elevated
privileges to do that.

Since the main purpose of `passwd` is to allow users change their passwords
and since it offers virtually no function that can be executed _without_
root privileges, set-uid ability seems like the best choice.

On the other hand, `dnf` can be run without root privileges (e.g., when
searching for packages) and hence it does not make good sense to always run
it under root. `dnf` itself could check whether the user is allowed to perform
given operation but it is simpler to let other program (such as `sudo`) do
that instead. It also helps in maintaining the security as the mechanism
(`sudo`) is kept separate from the programs and the same mechanism can be used
to control multiple programs (compared to having each program its own rules
who can run it).


If you think you answer contained the above and you have not received 25
points, please, check that you have not added some extra information that
is incorrect or whether your answers contains all the important parts. It is
possible we have overlooked something: feel free to open an issue or
discuss it with us during the labs.


Note that any reasoning that `passwd` changes only user password and does not
require root privileges is wrong as it modifies a system file (hence, it needs
root privileges).
Actually, for local users it would be probably possible to devise a system
where `passwd` does not need root privileges but the elevated privileges are
certainly needed when changing password over the network where only root can
access files with remote database passwords etc.


Also note that _file is run under root_ is rather strong shortcut. It is
unclear whether we are talking about accessing file from a processes running
under root or whether running executable (processes) is running with root
privileges.


Also note that the fact that a user can change his own password is not because
of set-uid but because the implementation of `passwd` says so. Also note that
`/usr/bin/passwd` is the command that modifies `/etc/passwd` and `/etc/shadow`
(usually).


## Q3 (packages)

**Points**: 25

**Expected:** 1, 5 (answered 1, 5)


## Q4 (signals and processes)

**Points**: 25

**Expected:** 1, 3, 4, 6, 8 (answered 1, 3, 4, 6, 8)


### General notes (collected from all solutions)

Based on the received answers, we realized that some of the questions
were not phrased as clearly as we wished them to be.

We therefore assigned points for any answer for several of the options
(so you may have 25 points even if our expected set does not match your
actual answer).

Here are our comments to some of the options to further clarify things.

Application can react in any way to any signal that it actually receives.
However, not all signals can be delivered to the application (e.g. SIGKILL
is never delivered and is handled by kernel directly because allowing this
signal to be handled by the application would defeat its purpose).

The signals are distinguished by their numbers only. We can also distinguish
them by the time they were delivered etc. but bare signal is just the number
(and for the operating system also the process the signal is delivered to).

A binary (i.e. a compiled program, such as from C) or a script is just a file.
Once we start this program (recall `./` and `$PATH`), operating system creates
a process. One program (binary or a script) can be started multiple times,
even at the same time (consider simple pipe `cut -d : -f 1 | cut -d , -f 3`).

There are also special kernel processes that are not backed by any physical
binary anywhere on the disk (actually, their code is in the kernel itself but
the point is that a normal user is unable to start his copy of such process
because there is no file to launch).

Each process has a unique identifier -- PID. The uniqueness applies to
currently running processes and eventually PIDs are reused. On normal systems
there is virtually no danger of reusing the PIDs so quickly that your `kill`
to a running program would kill a different one (i.e., the original target
of the signal meanwhile terminated and there was so many processes that
the PID is already reused and you kill a completely different process).



