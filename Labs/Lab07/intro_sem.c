 #include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <string.h>
 // how to use semaphore?
 #include <semaphore.h>

 #define THREAD_NUM 16

 sem_t semaphore;

 void* routine(void* args) {
    // before waiting
    prinf("(%d) Waiting in the login queue\n", *(int*)args);
    // sem_wait is going to check the semaphore 0
    // if sem == 0 the thread will wait on that semaphore
    // if s > 0 (maybe 5) it will decrement and start exectuing
    sem_wait(&semaphore);
    printf("(%d) Logged in\n", *(int*)args);
    sleep(rand() % 5 + 1);
    printf("(%d) Logged out\n", *(int*)args);
    // sem_post increment the sem value.
    sem_post(&semaphore);
    free(args);
 }
 
 
 
 int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];
    // initialize the semaphore
    // sem_init(&semaphore, -, -) -> &semaphore -> refernece to our semaphore variable
    // sem_init(-,0,-) -> if you have multiple processes or no
    // sem_init(-,-,1) -> initial value of the semaphore
    sem_init(&semaphore, 0, 12);

    // two things you can you with the semaphore inside the thread
    // wait & post
    // wait -> similar to lock in mutex
    // post -> similar to unlock in mutex
    int i;
    // thread creation
    for (i = 0; i < THREAD_NUM; i++) {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread!");
        }
    }

    // joining thread
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    // destory semaphore
    sem_destroy(&semaphore);
    return 0;
 }