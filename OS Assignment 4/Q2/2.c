#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int total_passengers;
int car_capacity;
int passengers_on_board = 0;
sem_t car_capacity_sem;
sem_t car_load;
sem_t car_unload;

void load();

void unload();

void board(int passenger_id);

void offboard(int passenger_id);

void *car(void *args);

void *passenger(void *args);

int main() {
    printf("Enter the total number of passengers: ");
    scanf("%d", &total_passengers);

    printf("Enter the car capacity: ");
    scanf("%d", &car_capacity);

    if (car_capacity >= total_passengers) {
        printf("Car capacity should be less than the total number of passengers.\n");
        return 1;
    }

    sem_init(&car_capacity_sem, 0, car_capacity);
    sem_init(&car_load, 0, 1);  // Initialize car_load semaphore with 1
    sem_init(&car_unload, 0, 1); // Initialize car_unload semaphore with 1


    pthread_t car_thread;
    pthread_create(&car_thread, NULL, car, NULL);

    pthread_t passenger_threads[total_passengers];
    int passenger_ids[total_passengers];

    for (int i = 0; i < total_passengers; i++) {
        passenger_ids[i] = i;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
    }
    pthread_join(car_thread, NULL);
    for (int i = 0; i < total_passengers; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    return 0;
}

void load() {
    printf("Car is loading passengers...\n");
    sleep(2);
}

void unload() {
    printf("Car is unloading passengers...\n");
    sleep(2);
}

void board(int passenger_id) {
    printf("Passenger %d is boarding the car.\n", passenger_id);
}

void offboard(int passenger_id) {
    printf("Passenger %d is getting off the car.\n", passenger_id);
}

void *car(void *args) {
    while (1) {
        sem_wait(&car_load);
        load();
        sem_post(&car_load);

        printf("Car is running...\n");
        sleep(2);

        sem_wait(&car_unload);
        printf("Car ride is over!\n");
        unload();
        sem_post(&car_unload);
    }

    return NULL;
}

void *passenger(void *args) {
    int passenger_id = *(int *)args;

    while (1) {
        sem_wait(&car_capacity_sem);


        if (passengers_on_board < car_capacity) {
            board(passenger_id);
            passengers_on_board++;
            if (passengers_on_board == car_capacity) {
                sem_post(&car_capacity_sem);
            }
        } else {
            sem_post(&car_capacity_sem);
            sleep(1);
            continue;
        }


        sleep(4);

        offboard(passenger_id);
        passengers_on_board--;


        sem_post(&car_capacity_sem);

        sleep(3);
    }

    return NULL;
}
