#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;

void eat(int philosopher) {
    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d is eating\n", philosopher);
    pthread_mutex_unlock(&print_lock);
    sleep(1);
}

void think(int philosopher) {
    pthread_mutex_lock(&print_lock);
    printf("Philosopher %d is thinking\n", philosopher);
    pthread_mutex_unlock(&print_lock);
    sleep(1);
}

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    
    for (int i = 0; i < 3; i++) {
        think(id);
        
        // Pick up left fork
        sem_wait(&forks[left_fork]);
        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d picked up left fork\n", id);
        pthread_mutex_unlock(&print_lock);
        
        // Pick up right fork
        sem_wait(&forks[right_fork]);
        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d picked up right fork\n", id);
        pthread_mutex_unlock(&print_lock);
        
        eat(id);
        
        // Put down forks
        sem_post(&forks[left_fork]);
        sem_post(&forks[right_fork]);
        pthread_mutex_lock(&print_lock);
        printf("Philosopher %d put down forks\n", id);
        pthread_mutex_unlock(&print_lock);
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];
    
    // Initialize semaphores (forks)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);
    }
    
    printf("Dining Philosophers Problem\n");
    printf("============================\n\n");
    
    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }
    
    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    // Cleanup semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
    
    printf("\nAll philosophers finished dining\n");
    return 0;
}
