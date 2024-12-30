# General Course Organization

The course is structured around a combination of lectures with self study
and project implementation. The self study instructions will come in the
form of weekly batches of self study material, which should prepare you
for the lectures and gradually provide the background knowledge necessary for
the project implementation. The project implementation is where you will
learn by doing - initially alone, later in teams of three, you will
implement your own operating system.

## Self Study

The instructions for self study will be distributed through this repository
every week on Fridays. Each self study instruction package will be in a
separate file, such as `01-architecture-study.md`. Read the file and follow
the instructions as you see fit - we fully expect you to skip those parts
that you already know, obviously there might also be parts where you will
need to look for additional background material on your own. See also the
[materials](#materials) section below.

We will primarily refer to the book by Arpaci-Dusseau et al. called Operating Systems: Three Easy Pieces.
This book is [available online](http://www.ostep.org) for free.
Reasonable initiative is expected in looking up additional material,
but the whole self study for one week should not take you more than
one hour - if you feel that is not enough to understand the content,
please bring your questions to the lecture for discussion.

## Weekly Quizzes

To regularly assess your progress, we will also post quizzes through this
repository every week on Fridays. Each quiz will be in a separate file, such
as `01-architecture-quiz.md`. Please answer the questions by editing the
file and committing it back into your repository, before the end of
Wednesday next week.

## Project Implementation

Independent of your self study activities, but synchronized so that the content
required for implementation is studied beforehand, you will work on a project
that will eventually culminate in a miniature but working operating system.

The project assignments will be posted in separate repositories with
announcement sent to the course mailing list. The labs will introduce
and discuss the assignments. Initially, you will work individually,
but in two weeks we will form teams of three students, to work
on the assignments throughout the semester.

Each project assignment will consists of a textual specification and automated tests.
The textual specification will include basic and extended requirements - the basic
requirements should be met so that your project has enough functionality for
the subsequent assignments, the extended requirements can be met for extra
points.

Each assignment will have a deadline, and you will need to tag a working solution
in the corresponding repository by that deadline. Tests used to evaluate your
solution will be provided together with the assignment and run on every
commit to provide immediate feedback on the state of your solution.


# Grading

The course grading is based on points from three sources:

- Knowledge Points
- Project Activity Points
- Project Implementation Points

## Knowledge Points

Knowledge points will be assigned for answering the weekly quizzes.
For each correct answer, you will receive a knowledge point.
You need to submit your answers before the deadline,
answers submitted late will not be considered.

## Project Activity Points

To distinguish the individual activity in the team project,
we will be using the repository commit logs from your team project.
For each day you commit on, you will receive one project activity point.
For each 100 lines of code you add, you will receive one project activity point.
The sum of all activity points will be capped at 20 points.

## Project Implementation Points

A tag must be assigned to the commit that represents the submitted solution in the corresponding repository.
Project implementation points are based on how the submitted solution
meets the assignment requirements - a maximum of 10 points is awarded
for meeting both basic and extended requirements, meeting
the basic requirements suffices for 8 points.

Automated tests will be used to evaluate each solution,
manual correction may be applied where tests fail
to expose serious errors.
The overall project implementation score will be computed as a sum of points received.


## Overall Grade

The points from the three sources will be normalized on a scale of 0 to 100 percent of maximum from each source.
To pass the course, you will need a minimum of 50 percent from each source,
the overall grade will be determined from the average of the three sources.
The *good* grade will be awarded for 50 percent or more,
the *very good* grade will be awarded for 70 percent or more,
the *excellent* grade will be awarded for 85 percent or more.

Please note that we try to keep the grading information compact and avoid legalese,
however, we will penalize attempts at gaming the system through formalistic interpretation.


# Materials

Our primary course book is [Arpaci-Dusseau et al.: Operating Systems: Three Easy Pieces (Version 1.00)](http://www.ostep.org).
We will also occassionally refer to the [MIPS R4000 Microprocessor User’s Manual (2nd Edition)](https://d3s.mff.cuni.cz/files/teaching/nswi004/resources/mips-processor.pdf)
and the [Intel 64 and IA-32 Architectures Software Developer's Manual (Version 325462-075)](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html).

If, for any reason, you need more information, there are other excellent books that can help you study.
Favorites include:

- Anderson et al.: Operating Systems Principles and Practice
- Silberschatz et al.: Operating System Concepts
- Tanenbaum et al.: Modern Operating Systems

None of these books follows the course exactly, but all contain enough material to suffice as a learning material.

A good book on the computer systems background is Patterson et al.: Computer Organization and Design.

For those who do not regularly work with Linux or C, we recommend
our [introduction to Linux](https://d3s.mff.cuni.cz/teaching/nswi177) and
our [system programming course](https://d3s.mff.cuni.cz/teaching/nswi170).


# Exceptional Situations

The following rules for handling exceptional situations concern
discretionary violations. Contact us if you think your situation
deserves special consideration. As a rule, we expect to be contacted
proactively rather than retroactively, unless your situation could not
have been foreseen.

## Late Quiz Submissions

Late quiz submissions will not be considered.

## Late Project Assignment Submission

Timely submissions will be graded after the submission deadline.
Late submissions will be accepted until the submission deadline
of the next assignment, with point penalty of 2 points.
Submissions past the next submission deadline will not be considered.

## Idle Team Members

Team members that did not commit for the duration of an entire assignment
will be ejected from their teams without redress. Incomplete teams will be
merged as the situation permits. Teams may arrange an alternative workload
distribution with the lecturers.


# Academic Integrity

The quiz work is individual, you should only discuss it with us.
Using online documentation is fine, using ready made solutions is not fine.

The project implementation is a team effort, you can discuss it both inside and outside your team.
The code you commit can be both internal, written by your team, or external, copied from other sources.

External code must be clearly marked as such, using external code without attribution is considered cheating.
If you decide to copy external code verbatim, state what part of your solution is copied and why.
If you decide to transform external code into your solution,
state what part of your solution was thus inspired and why.

When copying external code, think about what you are doing. Each assignment
asks you to demonstrate certain skill, but may involve additional work that
is needed simply to get things running. If the external code helps you with
the latter and the demonstration remains your own, fine. If you are
demonstrating skills of an external author, not fine.

When in doubt, ask us.

To avoid misunderstanding, here is a brief Q&A in the context of your first assignment:

**Q:** I found https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c. Is it fine if I copy the answer ?
**A:** No. The purpose of the assignment is to exercise your C coding skills. You demonstrate that by coding, not by copying.

**Q:** I found https://stackoverflow.com/questions/1735236/how-to-write-my-own-printf-in-c. Is it fine if I start with the answer and edit it into my own solution ?
**A:** Maybe. If you are new to C, one way to learn may be to get the existing code and play with it, changing and debugging things to get an understanding of how it works. If you end up with code that is significantly different from the original, and you state your original inspiration, then the assignment fulfilled its purpose and we will understand how you worked when grading. But you should really take the code apart until you understand every bit of it, and then combine the pieces back into your solution.

**Q:** I decided to extend the assignment and included the ability to print disassembly with %pD using https://github.com/Mithreindeir/Dynzasm. Is it fine ?
**A:** Sure, hope you had fun :-) did you remember to include attribution as required by the library license ?

Automated tests may fail to expose errors that violate the assignment requirements.
Exploiting such situations to simplify solutions is considered cheating.
