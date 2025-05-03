
// In the past quiz, several people asked about the thread creation overhead.
// Write a short program that creates threads that immediately terminate and measure
// how long this takes on the `lab` machine.

// You will want to check out `man pthread_create` for a way to create a thread,
// and `man pthread_join` for a way to wait for a thread to terminate. Also,
// there is a code sample that creates threads in the next self study text.

// Your answer should include your code and give the average time
// to create and wait for the termination of a single thread.

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