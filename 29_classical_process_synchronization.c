#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N 5
sem_t chopstick[N];
sem_t mutex;
int state[N]; 
#define THINKING 0
#define HUNGRY 1
#define EATING 2
void test(int i) {
    if (state[i] == HUNGRY && state[(i + N - 1) % N] != EATING && state[(i + 1) % N] != EATING) {
        state[i] = EATING;
        sem_post(&chopstick[i]);
    }
}
void pickup(int i) {
    sem_wait(&mutex);
    state[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);
    test(i);
    sem_post(&mutex);
    sem_wait(&chopstick[i]);
}
void putdown(int i) {
    sem_wait(&mutex);
    state[i] = THINKING;
    printf("Philosopher %d finished eating and is thinking\n", i);
    test((i + N - 1) % N);
    test((i + 1) % N);
    sem_post(&mutex);
}
void *philosopher(void *arg) {
    int i = *(int *)arg;
    for (int round = 0; round < 3; round++) {
        printf("Philosopher %d is thinking\n", i);
        sleep(rand() % 2 + 1);
        pickup(i);
        printf("Philosopher %d is eating (chopsticks: %d and %d)\n", i, i, (i + 1) % N);
        sleep(rand() % 2 + 1);
        putdown(i);
    }
    printf("Philosopher %d left the table\n", i);
    return NULL;
}
int main() {
    printf("======= Dining Philosophers Problem =======\n\n");
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&chopstick[i], 0, 0);
        state[i] = THINKING;
    }
    pthread_t philosophers[N];
    int philosopherIds[N];
    for (int i = 0; i < N; i++) {
        philosopherIds[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopherIds[i]);
        sleep(1); 
    }
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }
    sem_destroy(&mutex);
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopstick[i]);
    }
    printf("\nDining philosophers problem completed!\n");
    return 0;
}
