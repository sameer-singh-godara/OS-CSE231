#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5
#define BOWLS 2
int eats = 2;

pthread_mutex_t forks[NUM_PHILOSOPHERS] = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bowls_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t masterlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bowls_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bowls_mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t bowls_cond = PTHREAD_COND_INITIALIZER;

void philosopher(void* args) {
    int philosopher_id = *(int*)args;
    
    while (1){
        if (philosopher_id < NUM_PHILOSOPHERS -1){
            thinking(philosopher_id);
            pthread_mutex_lock(&masterlock);
            int left_fork = philosopher_id;
            int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

            pthread_mutex_lock(&forks[left_fork]);
            pthread_mutex_lock(&forks[right_fork]);
            while (pthread_mutex_trylock(&bowls_mutex1) != 0 && pthread_mutex_trylock(&bowls_mutex2) != 0){
                pthread_cond_wait(&bowls_cond, &masterlock);
            }
            pthread_mutex_unlock(&masterlock);

            eating(philosopher_id);

            pthread_mutex_unlock(&bowls_mutex1);
            pthread_mutex_unlock(&bowls_mutex2);
        
            pthread_mutex_unlock(&forks[left_fork]);
            pthread_mutex_unlock(&forks[right_fork]);
            
            pthread_cond_signal(&bowls_cond);
        }
        else {
            thinking(philosopher_id);
            pthread_mutex_lock(&masterlock);
            int left_fork = philosopher_id;
            int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

            pthread_mutex_lock(&forks[right_fork]);
            pthread_mutex_lock(&forks[left_fork]);
            while (pthread_mutex_trylock(&bowls_mutex1) != 0 && pthread_mutex_trylock(&bowls_mutex2) != 0){
                pthread_cond_wait(&bowls_cond, &masterlock);
            }
            pthread_mutex_unlock(&masterlock);

            eating(philosopher_id);

            pthread_mutex_unlock(&bowls_mutex1);
            pthread_mutex_unlock(&bowls_mutex2);
        
            pthread_mutex_unlock(&forks[right_fork]);
            pthread_mutex_unlock(&forks[left_fork]);
            
            pthread_cond_signal(&bowls_cond);
        }

    }
}

void eating(int philosopher_id) {
    // pthread_mutex_lock(&bowls_mutex);
    // int flag = 0;
    // while (1) {
    //     if (pthread_mutex_trylock(&bowls_mutex1) == 0) {
    //         flag = 1;
    //         break;
    //     }
    //     else if (pthread_mutex_trylock(&bowls_mutex2) == 0) {
    //         flag = 2;
    //         break;
    //     }
    //     pthread_cond_wait(&bowls_cond, &bowls_mutex);
    // }

    printf("Philosopher %d is eating\n", philosopher_id);
    sleep(1); // Simulate eating time

    // // Release the bowls
    // if (flag == 1) {
    //     pthread_mutex_unlock(&bowls_mutex1);
    // } else {
    //     pthread_mutex_unlock(&bowls_mutex2);
    // }
    // pthread_cond_signal(&bowls_cond);
    // pthread_mutex_unlock(&bowls_mutex);
}

void thinking(int id) {
    printf("philosopher %d is thinking\n", id);
    sleep(1); // Simulate thinking time
}

int main() {
    pthread_t philosopher_threads[NUM_PHILOSOPHERS];

    // Initialize mutexes and condition variable
    pthread_mutex_init(&bowls_mutex, NULL);
    pthread_mutex_init(&masterlock, NULL);
    pthread_mutex_init(&bowls_mutex1, NULL);
    pthread_mutex_init(&bowls_mutex2, NULL);
    pthread_cond_init(&bowls_cond, NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        int* philosopher_id = (int*)malloc(sizeof(int));
        *philosopher_id = i;
        pthread_create(&philosopher_threads[i], NULL, philosopher, philosopher_id);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    return 0;
}
