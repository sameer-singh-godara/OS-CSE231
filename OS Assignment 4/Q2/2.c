#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_PASSENGERS 1000

int total_passengers;
int capacity;
int passengers_on_board = 0;
int total_passengers_boarded_from_starting = 0;
int total_passengers_unboarded_from_starting = 0;
sem_t semaphore_mutex;
sem_t boardingQueue;
sem_t rideInProgress;
sem_t unboardingQueue;


void* car(void* args) {
    while (1) {
        // first Loading passengers
        load();

        // Signalling passengers to board onto the car
        for (int i = 0; i <= capacity; i++) {
            if (i == capacity){
                break;
            }
            sem_post(&boardingQueue);
            total_passengers_boarded_from_starting++;
        }

        // Wait for all passengers to board
        for (int i = 0; i <= capacity; i++) {
            if (i == capacity){
                break;
            }
            sem_wait(&rideInProgress);
        }

        // Simulate ride duration
        printf("\nCar: You are riding in the Safest Car, Ride in Progress\n");
        travelledPassenger();
        totalPassenger();
        sleep(1); // Simulating ride duration in the car

        // Unloading the passengers from car
        unload();

        // Signal passengers to unboard
        for (int i = 0; i <= capacity; i++) {
            if (i == capacity){
                break;
            }
            sem_post(&unboardingQueue);
            total_passengers_unboarded_from_starting++;
        }

        // Wait for all passengers to unboard the car
        for (int i = 0; i <= capacity; i++) {
            if (i == capacity){
                break;
            }
            sem_wait(&rideInProgress);
        }
    }

    return NULL;
}


void* passenger(void* args) {
    int passenger_id = *((int*)args);

    while (1) {
        passengerID(passenger_id);

        sem_wait(&boardingQueue);// Wait for the car to be ready for boarding
        board(passenger_id);// Boarding the car     

        sem_post(&rideInProgress);// Signal car that passenger has boarded

        sem_wait(&unboardingQueue);// Wait for the car to finish the ride        
        offboard(passenger_id);// Unboarding the car                     

        sem_post(&rideInProgress);// Signal car that passenger has unboarded

        passengerID(passenger_id);
    }

    return NULL;
}


void passengerID(int id){
    //printf("passenger id is %d\n", id);
}


void load() {
    sem_wait(&semaphore_mutex);
    printf("Car: Loading passengers\n");
    sleep(0.5); // Simulating loading time of passengers onto the car
    sem_post(&semaphore_mutex);
}


void unload() {
    sem_wait(&semaphore_mutex);
    printf("Car: Unloading passengers\n");
    sleep(0.5); // Simulating unloading time of passengers from the car
    sem_post(&semaphore_mutex);
}


void board(int passenger_id) {
    sem_wait(&semaphore_mutex);
    printf("Passenger %d: Boarding in the car\n", passenger_id);
    sleep(0.5); // Simulating boarding time
    sem_post(&semaphore_mutex);
}


void offboard(int passenger_id) {
    sem_wait(&semaphore_mutex);
    printf("Passenger %d: Getting off the car\n", passenger_id);
    sleep(0.5); // Simulating unboarding time
    sem_post(&semaphore_mutex);
}


void totalPassenger(){
    printf("The Total Number of Passengers Travelled till now is : %d\n", total_passengers_boarded_from_starting);
    travelledPassenger();
}

void travelledPassenger(){
    int count = 0;
    for (int i = 0; i <= total_passengers_boarded_from_starting; i++){
        count++;
    }
}


int main() {
    // Input: Total passengers and car capacity
    printf("Enter total number of passengers: ");
    scanf("%d", &total_passengers);

    printf("Enter car capacity: ");
    scanf("%d", &capacity);

    if (capacity >= total_passengers){
        printf("There is are less passengers as compared to the capacity of car\n");
        return 1;
    }
    // Initialize semaphores
    sem_init(&semaphore_mutex, 0, 1);
    sem_init(&boardingQueue, 0, 0);
    sem_init(&rideInProgress, 0, 0);
    sem_init(&unboardingQueue, 0, 0);

    // Create car thread
    pthread_t car_thread;
    pthread_create(&car_thread, NULL, car, NULL);
    printf("CAR THREAD CREATED SUCCESSFULLY\n");

    // Create passenger threads
    pthread_t passenger_threads[MAX_PASSENGERS];
    int passenger_ids[MAX_PASSENGERS];

    for (int i = 0; i <= total_passengers; i++) {
        if (i == total_passengers){
            break;
        }
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
        // printf("passenger %d thread created successfully\n", i);
    }

    // Join threads
    pthread_join(car_thread, NULL);

    for (int i = 0; i <= total_passengers; i++) {
        if (i == total_passengers){
            printf("Now all threads are joined with the parent thread\n");
            break;
        }
        pthread_join(passenger_threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&semaphore_mutex);
    sem_destroy(&boardingQueue);
    sem_destroy(&rideInProgress);
    sem_destroy(&unboardingQueue);

    return 0;
}