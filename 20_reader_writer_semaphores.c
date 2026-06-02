#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 4
#define NUM_WRITERS 2
#define NUM_READS 3
#define NUM_WRITES 2

int read_count = 0;
int write_count = 0;
int shared_data = 0;

sem_t mutex;        // Protects read_count and write_count
sem_t write_mutex;  // Controls writer access
sem_t read_sem;     // Controls reader access

void *reader(void *arg) {
    int reader_id = *(int *)arg;
    
    for (int i = 0; i < NUM_READS; i++) {
        // Wait for access
        sem_wait(&read_sem);
        
        // Critical section - Update read count
        sem_wait(&mutex);
        read_count++;
        
        if (read_count == 1) {
            // First reader blocks writers
            sem_wait(&write_mutex);
        }
        sem_post(&mutex);
        
        sem_post(&read_sem);
        
        // Reading section
        printf("Reader %d: Reading shared data = %d\n", reader_id, shared_data);
        sleep(1);
        
        // Critical section - Update read count
        sem_wait(&mutex);
        read_count--;
        
        if (read_count == 0) {
            // Last reader releases writers
            sem_post(&write_mutex);
        }
        sem_post(&mutex);
    }
    
    printf("Reader %d: Finished\n", reader_id);
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *(int *)arg;
    
    for (int i = 0; i < NUM_WRITES; i++) {
        // Wait for write access
        sem_wait(&write_mutex);
        
        // Critical section - Write section
        shared_data++;
        printf("Writer %d: Writing shared data = %d\n", writer_id, shared_data);
        sleep(2);
        
        // Release write access
        sem_post(&write_mutex);
    }
    
    printf("Writer %d: Finished\n", writer_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int reader_ids[NUM_READERS];
    int writer_ids[NUM_WRITERS];
    
    printf("Reader-Writer Problem Using Semaphores\n");
    printf("======================================\n");
    printf("Number of readers: %d (each reads %d times)\n", NUM_READERS, NUM_READS);
    printf("Number of writers: %d (each writes %d times)\n", NUM_WRITERS, NUM_WRITES);
    printf("Initial shared data: %d\n\n", shared_data);
    
    // Initialize semaphores
    sem_init(&mutex, 0, 1);       // Binary semaphore
    sem_init(&write_mutex, 0, 1); // Binary semaphore
    sem_init(&read_sem, 0, 1);    // Binary semaphore
    
    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }
    
    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }
    
    // Wait for all readers to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    
    // Wait for all writers to finish
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }
    
    // Cleanup
    sem_destroy(&mutex);
    sem_destroy(&write_mutex);
    sem_destroy(&read_sem);
    
    printf("\n========== Final State ==========\n");
    printf("Final shared data value: %d\n", shared_data);
    printf("Expected value: %d (initial 0 + %d writes)\n", NUM_WRITERS * NUM_WRITES, NUM_WRITERS * NUM_WRITES);
    printf("\nReader-Writer execution completed successfully!\n");
    
    printf("\n========== Explanation ==========\n");
    printf("- Multiple readers can access data simultaneously\n");
    printf("- Writers have exclusive access\n");
    printf("- First reader blocks writers\n");
    printf("- Last reader unblocks writers\n");
    
    return 0;
}
