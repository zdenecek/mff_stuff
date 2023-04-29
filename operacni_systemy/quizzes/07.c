
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


static pthread_cond_t cond; // Initialized elsewhere.
static pthread_mutex_t mutex; // Initialized elsewhere.

atomic_int x = 0;

void *thread_one_function (void *dummy) {
    while (true) {
        pthread_mutex_lock (&mutex);
        x++;
        if (x == 10) {
            pthread_cond_signal (&cond);
        }
        pthread_mutex_unlock (&mutex);
    }
}

void *thread_two_function (void *dummy) {
    pthread_mutex_lock (&mutex);
    pthread_cond_wait (&cond, &mutex);
    printf ("%d\n", x);
    pthread_mutex_unlock (&mutex);
}


int main(int argc, char *argv[]) {
    pthread_t threads[2];
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&threads[0], NULL, thread_one_function, (void *)NULL);
    pthread_create(&threads[1], NULL, thread_two_function, (void *)NULL);

    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
   pthread_exit(NULL);
}