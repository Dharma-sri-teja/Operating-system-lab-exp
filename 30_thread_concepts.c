#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
int shared_counter = 0;
pthread_t thread_ids[5];
int thread_count = 0;
void *thread_function_1(void *arg) {
    int thread_id = (intptr_t)arg;
    printf("Thread %d: Started execution\n", thread_id);
    for (int i = 0; i < 3; i++) {
        shared_counter++;
        printf("Thread %d: Counter = %d\n", thread_id, shared_counter);
        sleep(1);
    }
    printf("Thread %d: Exiting\n", thread_id);
    pthread_exit((void *)(intptr_t)(thread_id * 10)); 
}
void *thread_function_2(void *arg) {
    int thread_id = (intptr_t)arg;
    printf("Thread %d: Started execution\n", thread_id);
    for (int i = 0; i < 2; i++) {
        printf("Thread %d: Working...\n", thread_id);
        sleep(1);
    }
    printf("Thread %d: Exiting\n", thread_id);
    return NULL;
}
void demo_create() {
    printf("\n===== Thread Creation Demo =====\n");
    printf("Creating threads...\n\n");
    for (int i = 1; i <= 3; i++) {
        int rc = pthread_create(&thread_ids[i], NULL, thread_function_2, (void *)(intptr_t)i);
        if (rc) {
            printf("Error: unable to create thread\n");
            exit(1);
        }
        printf("Main: Created thread %ld\n", thread_ids[i]);
    }
    thread_count = 3;
}
void demo_join() {
    printf("\n===== Thread Join Demo =====\n");
    printf("Waiting for threads to complete...\n\n");
    for (int i = 1; i <= 3; i++) {
        void *exit_status;
        int rc = pthread_join(thread_ids[i], &exit_status);
        if (rc) {
            printf("Error: unable to join thread\n");
            exit(1);
        }
        printf("Main: Joined thread %ld (exit status: %s)\n", thread_ids[i], (exit_status == NULL) ? "NULL" : (char *)exit_status);
    }
    printf("All threads completed.\n");
}
void demo_equal() {
    printf("\n===== Thread Equal Demo =====\n");
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread_function_2, (void *)(intptr_t)1);
    pthread_create(&thread2, NULL, thread_function_2, (void *)(intptr_t)2);
    printf("thread1 ID: %ld\n", thread1);
    printf("thread2 ID: %ld\n", thread2);
    if (pthread_equal(thread1, thread2)) {
        printf("Threads are equal (same)\n");
    } else {
        printf("Threads are not equal (different)\n");
    }
    if (pthread_equal(thread1, thread1)) {
        printf("thread1 is equal to itself\n");
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}
void *worker_thread(void *arg) {
    int worker_id = (intptr_t)arg;
    printf("Worker %d: Starting\n", worker_id);
    if (worker_id == 2) {
        printf("Worker %d: Exiting early!\n", worker_id);
        pthread_exit((void *)(intptr_t)100); 
    }
    for (int i = 0; i < 5; i++) {
        printf("Worker %d: Step %d\n", worker_id, i);
        sleep(1);
    }
    printf("Worker %d: Completed normally\n", worker_id);
    return NULL;
}
void demo_exit() {
    printf("\n===== Thread Exit Demo =====\n");
    pthread_t workers[3];
    int rc;
    for (int i = 1; i <= 3; i++) {
        rc = pthread_create(&workers[i], NULL, worker_thread, (void *)(intptr_t)i);
        if (rc) {
            printf("Error creating thread\n");
            exit(1);
        }
    }
    for (int i = 1; i <= 3; i++) {
        void *exit_status;
        rc = pthread_join(workers[i], &exit_status);
        if (rc) {
            printf("Error joining thread\n");
            exit(1);
        }
        printf("Main: Worker %d exited with status: %ld\n", i, (intptr_t)exit_status);
    }
}
void demo_all_concepts() {
    printf("\n===== Combined Thread Operations Demo =====\n");
    pthread_t threads[2];
    printf("\n[CREATE] Creating threads...\n");
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_function_1, (void *)(intptr_t)(i + 1));
    }
    printf("\n[EQUAL] Comparing threads...\n");
    if (pthread_equal(threads[0], threads[1])) {
        printf("Threads are equal\n");
    } else {
        printf("Threads are different\n");
    }
    printf("\n[JOIN] Waiting for threads to complete...\n");
    for (int i = 0; i < 2; i++) {
        void *exit_status;
        pthread_join(threads[i], &exit_status);
        printf("Thread %d returned: %ld\n", i, (intptr_t)exit_status);
    }
    printf("\n[EXIT] All threads have exited normally.\n");
    printf("Final counter value: %d\n", shared_counter);
}
int main() {
    printf("======= Thread Concepts Demonstration =======\n");
    demo_create();
    demo_join();
    sleep(1);
    demo_equal();
    sleep(1);
    demo_exit();
    sleep(1);
    demo_all_concepts();
    printf("\n======= All demonstrations completed =======\n");
    return 0;
}
