#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"
#include "buffer.h"
#include <time.h>

/*the buffer*/
buffer_item buffer[BUFFER_SIZE];
int front = 0, rear = 0;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insert_item(buffer_item item) {

    // we will wait for the empty slot
    sem_wait(&empty);

    pthread_mutex_lock(&mutex);
    /* insert an item into buffer */
    buffer[rear] = item;
    rear = (rear + 1) % BUFFER_SIZE;
    printf("producer produced %d\n", item);

    // then release it
    pthread_mutex_unlock(&mutex);
    sem_post(&full);

    return 0;

    /* return 0 if successful, otherwise
       return -1 indicating an error condition
    */
}
int remove_item(buffer_item *item) {
        /* remove an object from buffer and placing it in item*/
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        // removing the item now
        *item = buffer[front];
        front = (front + 1) % BUFFER_SIZE;
        printf("consumer consumed %d\n", *item);

        // release
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        return 0;

        /* return 0 if successful, otherwise
            return -1 indicating an error condition
        */
}

void *producer(void *param) {
    buffer_item item;
    // will be approaching with seed fro randomization
    unsigned int seed = time(NULL)^pthread_self();

    while (1) {
        /* sleep for a random period of time */
        // the sleep will be 1-3 seconds
        sleep(rand_r(&seed) % 3 + 1);

        /* generate a random number */ 
        item = rand_r(&seed);
        if (insert_item(item) < 0) printf("Error inserting item\n");
    }
}

void *consumer(void *param) {
    buffer_item item;

    unsigned int seed = time(NULL)^pthread_self();
    while (1)
    {
        /* sleep for a randmo periof of time*/
        sleep(rand_r(&seed) & 3 + 1);
        if (remove_item(&item) < 0) printf("Error removing item\n");
        /* code */
    }
    
}

int main(int argc,char* argv[]) {
    /* get command line arguments argv[0-3] */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <sleep time> <producer threads> <consumer threads\n", argv[0]);

        exit(1);
    }
    /* initialiez buffers mutex semaphore */
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    /* create producer threads */
    pthread_t producers[num_producers];
    for (int i = 0; i < num_producers; i++) {
        if (pthread_create(&producers[i], NULL, producer, NULL) != 0) {
            fprintf(stderr, "Failed to create producer thread %d\n", i);
            exit(1);
        }
    }

    /* create consumer threads */
    pthread_t consumers[num_consumers];
    for (int j = 0; j < num_consumers; j++) {
        if (pthread_create(&consumers[j], NULL, consumer, NULL) != 0) {
            fprintf(stderr, "Failed to create consumer thread %d\n", j);
            exit(1);
        }
    }


    /* sleep */
    sleep(sleep_time);

    // cleanup

    // cancel producer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_cancel(producers[i]);
    }
    
    // cancel consumer threads
    for (int j = 0; j < num_consumers; j++) {
        pthread_cancel(consumers[j]);
    }

    // producer thread join
    for (int i = 0; i < num_producers; i++) {
        if (pthread_join(producers[i], NULL) != 0) {
            fprintf(stderr, "pthread producer %d failed to join\n", i);
        }
    }

    // consumer thread join
    for (int j = 0; j < num_consumers; j++) {
        if (pthread_join(consumers[j], NULL) != 0) {
            fprintf(stderr, "pthread consumer %d failed to join", j);
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    /*
    mutex
    sem1
    sem2
    ????
    des sem2
    des sem1
    des mutex
    */
    

    // Exit
    printf("Program finished.\n");
    return 0;
}