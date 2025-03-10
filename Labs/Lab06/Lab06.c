#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int mail = 0;
pthread_mutex_t mutex;

void* routine() {
    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[10];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perrro("Failed to create thread")''
            return 1;
        }
        printf("Thread %d has started\n", i);
    }
    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has finished execution\n", i);
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mail);
    return 0;
}