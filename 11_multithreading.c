#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void *thread_function(void *arg) {
    int thread_id = *(int *)arg;
    printf("Thread %d started\n", thread_id);
    
    for (int i = 1; i <= 3; i++) {
        printf("Thread %d: iteration %d\n", thread_id, i);
        sleep(1);
    }
    
    printf("Thread %d finished\n", thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    
    printf("Main: Creating %d threads\n", NUM_THREADS);
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        int rc = pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    // Wait for all threads to complete
    printf("Main: Waiting for threads to complete\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Main: All threads completed\n");
    return 0;
}
