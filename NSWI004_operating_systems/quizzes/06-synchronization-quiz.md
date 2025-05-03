# Process Synchronization

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of November 9).


## Concurrency

**[Q1]** Assume the following implementation of a simple bit set in C:

```c
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define BIT_SET_SIZE 1024

typedef uint8_t bit_set_t [BIT_SET_SIZE / 8];

void set_add (bit_set_t set, unsigned int value) {
    assert (value < BIT_SET_SIZE);
    size_t offset = value / 8;
    uint8_t mask = 1 << (value % 8);
    set [offset] |= mask;
}

void set_del (bit_set_t set, unsigned int value) {
    assert (value < BIT_SET_SIZE);
    size_t offset = value / 8;
    uint8_t mask = 1 << (value % 8);
    set [offset] &= ~mask;
}
```

The implementation is not safe to use with multiple threads.
Explain the race condition that can occur when `set_add` or
`set_del` is called from two threads at the same time.

**[A1]** The assignment operators |= and &= are not atomic. The switch could occur right after reading the set variable and then the old values would be reassigned after the function execution is resumed. **[/A1]**


## Atomic Operations

**[Q2]** Assume the existence of a `bool __atomic_compare_exchange (type *ptr, type *expected, type desired)` function,
which compares `*expected` with `*ptr`. If equal, it writes `desired` into `*ptr` and returns `true`, otherwise
it writes `*ptr` into `*expected` and returns `false`. These steps happen atomically.

(This happens to match [the definition of the GCC built in atomic function](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html) except for the `weak` and `memorder` parameters.)

Using the `__atomic_compare_echange` function, update the implementation of the `set_add` function
from the previous question to eliminate the race the previous question asked about.

**[A2]** 
```c
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define BIT_SET_SIZE 1024

typedef uint8_t bit_set_t [BIT_SET_SIZE / 8];

void set_add (bit_set_t set, unsigned int value) {
    assert (value < BIT_SET_SIZE);

    uint8_t* a = NULL; 

    size_t offset = value / 8;
    uint8_t initial_value = set [offset];
    uint8_t mask = 1 << (value % 8);

    for(;;) {   
        uint8_t new_value = set [offset] | mask;
        
        // I dont know that the last parameter does, it was not described, this should work, however.
        if(__atomic_compare_exchange (set + offset, &initial_value, new_value, a)) break;

}
```
**[/A2]**


## Locks

**[Q3]** The following program uses locks to implement a thread ping pong pattern,
similar to the one with semaphores used during the lecture when measuring
the context switch time (error checking omitted for brevity):

```c
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t ping_mutex, pong_mutex;

void *ping_thread_body (void *arguments) {
    while (true) {
        pthread_mutex_lock (&ping_mutex);
        pthread_mutex_unlock (&pong_mutex);
    }
}

void *pong_thread_body (void *arguments) {
    while (true) {
        pthread_mutex_unlock (&ping_mutex);
        pthread_mutex_lock (&pong_mutex);
    }
}

int main () {
    // Initialize the mutexes.
    pthread_mutex_init (&ping_mutex, NULL);
    pthread_mutex_init (&pong_mutex, NULL);
    pthread_mutex_lock (&ping_mutex);
    pthread_mutex_lock (&pong_mutex);

    // Launch two threads that ping pong.
    pthread_t ping_thread, pong_thread;
    pthread_create (&ping_thread, NULL, ping_thread_body, NULL);
    pthread_create (&pong_thread, NULL, pong_thread_body, NULL);

    // Wait indefinitely for the two threads to finish.
    pthread_join (ping_thread, NULL);
    pthread_join (pong_thread, NULL);
}
```

Will the threads ping pong (indefinitely) when the program is run ? Explain why.

(You may want to consult `man pthread_mutex_lock` for details.)

**[A3]** In the referenced manual, it is said that "Attempting to unlock the mutex if it was not locked by the calling thread results in undefined behavior.". So the behaviour is undefined. Otherwise, the threads will never stop execution. **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** What are some common issues that come up when using condition variables? **[/A0]**
