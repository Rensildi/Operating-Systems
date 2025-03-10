#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/*

*/

pthread_mutex_t mutexFuel;
pthread_cond_t condFuel;
int fuel = 0;

void* fule_filling(void* arg) {
    for (int i=0;i < 5; i++ ) {
        pthread_mutex_lock(&mutexFuel);
        fuel += 60;
        printf("Filled fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutexFuel);
        // awaken all the threads 
        pthread_cond_broadcast(&condFuel);
        sleep(1);
    }
}

void* car(void* arg) {
    pthread_mutex_lock(&mutexFuel);
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");
        pthread_cond_wait(&condFuel, &mutexFuel);
        // Equivalent to:
        // pthread_mutex_unlock(&mutexFuel);
        // wait for signal on condFuel
        // pthread_mutex_lock(&mutexFuel);
    }
    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutexFuel);
}


int main(int argc, char* argv[]) {
    pthread_t th[5]
    pthread_mutex_init(&mutexFuel, NULL);
    for (int i = 0; i < 5; i++) {
        if (i == 4) {
            if (pthread_create(&th[i], NULL, &fule_filling, NULL) != 0) {
                perror("Failed to create thread!");
            } else {
                if (pthread_create(&th[i], NULL, &car, NULL ) != 0) {
                    perror("Failed to create thread!");
                }
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread!");
        }
    }
    pthread_mutex_destroy(&mutexFuel);
    return 0;
}