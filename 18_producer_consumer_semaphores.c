#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_COUNT 3

int buffer[BUFFER_SIZE];
int count = 0; // Current items in buffer
int in = 0;   // Next position to insert
int out = 0;  // Next position to remove

sem_t empty;   // Semaphore for empty slots
sem_t full;    // Semaphore for full slots
pthread_mutex_t mutex; // Mutex for critical section

void *producer(void *arg) {
    int producer_id = *(int *)arg;
    
    for (int i = 0; i < ITEMS_COUNT; i++) {
        int item = producer_id * 100 + i;
        
        // Wait for empty slot
        sem_wait(&empty);
        
        // Critical section
        pthread_mutex_lock(&mutex);
        
        buffer[in] = item;
        printf("Producer %d produced item %d at position %d (Buffer: %d/%d)\n", 
               producer_id, item, in, count + 1, BUFFER_SIZE);
        
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        pthread_mutex_unlock(&mutex);
        
        // Signal full slot
        sem_post(&full);
        
        sleep(1); // Simulate production time
    }
    
    printf("Producer %d finished\n", producer_id);
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int consumer_id = *(int *)arg;
    
    for (int i = 0; i < ITEMS_COUNT; i++) {
        // Wait for full slot
        sem_wait(&full);
        
        // Critical section
        pthread_mutex_lock(&mutex);
        
        int item = buffer[out];
        printf("Consumer %d consumed item %d from position %d (Buffer: %d/%d)\n", 
               consumer_id, item, out, count - 1, BUFFER_SIZE);
        
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        pthread_mutex_unlock(&mutex);
        
        // Signal empty slot
        sem_post(&empty);
        
        sleep(2); // Simulate consumption time
    }
    
    printf("Consumer %d finished\n", consumer_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int prod_ids[NUM_PRODUCERS];
    int cons_ids[NUM_CONSUMERS];
    
    printf("Producer-Consumer Problem Using Semaphores\n");
    printf("==========================================\n");
    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producers: %d, Consumers: %d\n", NUM_PRODUCERS, NUM_CONSUMERS);
    printf("Items per producer: %d\n\n", ITEMS_COUNT);
    
    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE); // Buffer is initially empty
    sem_init(&full, 0, 0);             // No items initially
    pthread_mutex_init(&mutex, NULL);
    
    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &prod_ids[i]);
    }
    
    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &cons_ids[i]);
    }
    
    // Wait for all producers and consumers to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }
    
    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    printf("\nProducer-Consumer execution completed successfully!\n");
    
    return 0;
}
