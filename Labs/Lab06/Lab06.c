/*
    Create a program where 10 'worker' threads are created.
    Set all of the worker threads to block on the same condition
    variable and output that the thread is blocked. Last, use
    pthread_cond_broadcast to signal all worker threads to wake
    up at the same time and output when each thread is unblocked
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

// global variables
pthread_mutex_t mutex; // lock & unlock the data
pthread_cond_t cond; // block and wake up the threads
int count = 0; // keeping track of the blocked threads

void* status(void* arg) {
    // id is initialized with a dereference to retrieve the thread number
    // the reason for creating an id for each thread is because I realized I was using only one thread before
    // showing all Blocked 0 thread 
    // The issue was with the wait condition, I was not initializing correct
    // giving an id to each thunder to track helped.
    int id = *(int*)arg;
    // free is always used everytime malloc has been initialized 
    // prevents memory leaks
    // it frees the memory that is dynamically allocated
    free(arg);  // Free dynamically allocated memory
    pthread_mutex_lock(&mutex); // lockign the mutex
    // printing the thread with the id to keep track of which one is executed
    printf("Thread %d Blocked\n", id);
    count += 1; // keeping track of the blocked threads

    // While loop will ensure all the threads are blocked before broadcasting
    // the threads will keep waiting until count reaches 10
    while (count < 10) {
        // mutex will be unlocked during the wait
        pthread_cond_wait(&cond, &mutex);
    }
    // once the data is modified the mutex is unlocked
    pthread_mutex_unlock(&mutex);

    // printing the threads that are being unblocked
    printf("Thread %d Awakened\n", id);

}

int main(int argc, char* argv[]) {
    pthread_t th[10];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    // creating threads
    for (int i = 0; i < 10; i++) {
        int* id = malloc(sizeof(int)); // Allocate memory for thread id 
        // each id will be unique
        *id = i + 1;
        if (pthread_create(&th[i], NULL, &status, id) != 0) {
            // setting up logs for errors
            perror("Failed to create thread!");
        }
    }

    // allowing time of 1 second for all threads to block
    sleep(1);

    // wake up all threads
    pthread_mutex_lock(&mutex); // lock the mutex before modiying data
    pthread_cond_broadcast(&cond); // signal all threads to wake them up
    pthread_mutex_unlock(&mutex); // then mutex will unlock after broadcasting
    
    // joining threads 
    // will ensure the main thread to wait for others to finish
    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0)  {
            // setting up logs for errors
            perror("Failed to join thread!");
        }
    }
    // destorying the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}