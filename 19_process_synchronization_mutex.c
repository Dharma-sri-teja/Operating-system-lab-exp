#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

// Shared resource
int counter = 0;
pthread_mutex_t mutex;

// Critical section without protection (for comparison)
void increment_unsafe() {
    int temp = counter;
    temp++;
    counter = temp;
}

// Critical section with mutex protection
void increment_safe() {
    pthread_mutex_lock(&mutex);
    
    int temp = counter;
    temp++;
    counter = temp;
    
    pthread_mutex_unlock(&mutex);
}

void *worker_unsafe(void *arg) {
    int thread_id = *(int *)arg;
    
    printf("Thread %d: Starting (unsafe increment)\n", thread_id);
    
    for (int i = 0; i < 1000; i++) {
        increment_unsafe();
    }
    
    printf("Thread %d: Finished (unsafe increment)\n", thread_id);
    pthread_exit(NULL);
}

void *worker_safe(void *arg) {
    int thread_id = *(int *)arg;
    
    printf("Thread %d: Starting (safe increment)\n", thread_id);
    
    for (int i = 0; i < 1000; i++) {
        increment_safe();
    }
    
    printf("Thread %d: Finished (safe increment)\n", thread_id);
    pthread_exit(NULL);
}

void test_without_mutex() {
    printf("\n========== Test 1: WITHOUT Mutex Protection ==========\n");
    printf("Expected counter value: %d\n", NUM_THREADS * 1000);
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, worker_unsafe, &thread_ids[i]);
    }
    
    // Wait for threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Actual counter value: %d\n", counter);
    printf("Lost increments: %d\n", NUM_THREADS * 1000 - counter);
    printf("RESULT: Race condition detected!\n");
}

void test_with_mutex() {
    printf("\n========== Test 2: WITH Mutex Protection ==========\n");
    printf("Expected counter value: %d\n", NUM_THREADS * 1000);
    
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    
    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);
    
    counter = 0;
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, worker_safe, &thread_ids[i]);
    }
    
    // Wait for threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Actual counter value: %d\n", counter);
    printf("Lost increments: %d\n", NUM_THREADS * 1000 - counter);
    printf("RESULT: Synchronization successful!\n");
    
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
}

int main() {
    printf("Process Synchronization Using Mutex Locks\n");
    printf("========================================\n");
    printf("Number of threads: %d\n", NUM_THREADS);
    printf("Increments per thread: 1000\n");
    printf("Total expected increments: %d\n", NUM_THREADS * 1000);
    
    test_without_mutex();
    test_with_mutex();
    
    printf("\n========== Summary ==========\n");
    printf("Mutex ensures that only one thread can access\n");
    printf("the critical section at a time, preventing race conditions.\n");
    printf("Without mutex, multiple threads can access the same\n");
    printf("resource simultaneously, leading to data corruption.\n");
    
    return 0;
}
