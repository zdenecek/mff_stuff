# Process Scheduling

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of November 2).


## Parallelism

**[Q1]** Assume a system with a single processor and two running processes.
One is a text editor, one is a music player. Both processes are scheduled
with a simple priority based scheduler - a process has a fixed priority,
defined as an integer, and the scheduler always runs the process with
the highest priority. If multiple processes have the same priority,
the scheduler runs them in a round robin fashion, with a fixed
quantum of 10 milliseconds.

Propose (justify) an assignment of priorities to the two processes.

Assuming your priority settings are used, describe how
the system would behave in these three scenarios:

1. The text editor requires 1 millisecond bursts of processing power every time
   the user presses a key on the keyboard, which happens around once per
   1 second. The music player requires 10 milliseconds of processing to
   prepare a sound buffer that plays for the next 100 milliseconds.

2. In addition to the first point, the text editor requires 1 second bursts of
   processing power on some of the key presses, which happen to invoke more
   complex text editor functions.

3. In contrast to the first point, the music player requires 150 milliseconds
   of processing to prepare a sound buffer that plays for the next 100 milliseconds.

**[A1]** I would assign a higher priority to the music player, as music should not stop playing and has priority over text editing.
In the first scenario, per one second we need 100ms of processing time for the music player and on average 1ms for the text editor, that seems OK.
In the second scenario, in some cases the music player will take priory over the editor, which is as it should be, the editor will still have 90% of the shared time.
Int the third scenario, the music player will take up all the capacity and still not play continously. As we prioritized playing music continously, we have to face this consequence. Normally however, a music player should take less than over 100% of processing time, moreover, even if it does, we can prempt it with a more complex scheduler. **[/A1]**


## Processes

**[Q2]** Imagine one program that prints an address of an integer variable,
and another that reads an address of an integer variable and prints that variable:

```c
// Program ONE

#include <stdio.h>
#include <inttypes.h>

int main (void) {
    int value = 0xBAADFEED;
    printf ("%" PRIdPTR "\n", &value);
    while (!0) { };
}
```

```c
// Program TWO

#include <stdio.h>
#include <inttypes.h>

int main (void) {
    int *address;
    scanf ("%" SCNdPTR, &address);
    printf ("%x\n", *address);
}
```

When you run the two programs and copy the output of ONE to the input of TWO, what will be printed ? Explain why.

**[A2]** The program will not behave as the author might have supposed. The processes don't share adress space and so the addresses are invalid outside of the process scope. **[/A2]**


## Threads

**[Q3]** In the past quiz, several people asked about the thread creation overhead.
Write a short program that creates threads that immediately terminate and measure
how long this takes on the `lab` machine.

You will want to check out `man pthread_create` for a way to create a thread,
and `man pthread_join` for a way to wait for a thread to terminate. Also,
there is a code sample that creates threads in the next self study text.

Your answer should include your code and give the average time
to create and wait for the termination of a single thread.

**[A3]** 
'''c
#include <time.h>
#include <stdio.h>
#include <pthread.h>
void* run(void* i){
    //printf("%d\n", (int)i);
}

# define COUNT 10000

int main(void) {
    clock_t start, end;
    double time_used;
    start = clock();
    pthread_t pool[COUNT];
    for (int i = 0; i < COUNT; i ++){
        pthread_create(&pool[i], NULL, run, NULL);
    }

    for (int i = 0; i < COUNT; i ++){
        pthread_join(pool[i], NULL);
    }

    end = clock();
    time_used = ((double) (end - start) )/ COUNT / CLOCKS_PER_SEC;
    printf("%lf\n", time_used);

    return 0;
} 
'''

Average time: 0.065 miliseconds

 **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** How can the use of locks lead to deadlocks? **[/A0]**
