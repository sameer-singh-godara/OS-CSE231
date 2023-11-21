#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_BOWLS 2

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t bowls[NUM_BOWLS];
pthread_cond_t cond[NUM_PHILOSOPHERS];

void eating(int philosopher_id) {
    printf("Philosopher %d is eating\n", philosopher_id);
    usleep(rand() % 1000000); // Simulate eating time
}

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking\n", philosopher_id);
    usleep(rand() % 1000000); // Simulate thinking time
}

void* philosopher(void* args) {
    int philosopher_id = *(int*)args;

    while (1) {
        // Thinking
        thinking(philosopher_id);

        // Pick up forks
        pthread_mutex_lock(&forks[philosopher_id]);
        pthread_mutex_lock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);

        // Pick up bowls
        pthread_mutex_lock(&bowls[0]);
        pthread_mutex_lock(&bowls[1]);

        // Enter eating state
        eating(philosopher_id);

        // Release forks
        pthread_mutex_unlock(&forks[philosopher_id]);
        pthread_mutex_unlock(&forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]);

        // Release bowls
        pthread_mutex_unlock(&bowls[0]);
        pthread_mutex_unlock(&bowls[1]);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes and condition variables
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
        pthread_mutex_init(&cond[i], NULL);
    }

    for (int i = 0; i < NUM_BOWLS; i++) {
        pthread_mutex_init(&bowls[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Wait for threads to finish (this will never happen in this example)
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes and condition variables
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&cond[i]);
    }

    for (int i = 0; i < NUM_BOWLS; i++) {
        pthread_mutex_destroy(&bowls[i]);
    }

    return 0;
}
