#include "pthread.h"
#include "stdio.h"
#include "stdlib.h"
#include "semaphore.h"
#include "buffer.h"

/*the buffer*/
buffer_item buffer[BUFFER_SIZE];
int n;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insert_item(buffer_item item) {
    /* insert an item into buffer */
    printf("producer produced %d\n", item);

    /* return 0 if successful, otherwise
       return -1 indicating an error condition
    */
}
int remove_item(buffer_item *item) {
        /* remove an object from buffer and placing it in item*/
        
        printf("consumer consumed %d\n", rand);

        /* return 0 if successful, otherwise
            return -1 indicating an error condition
        */
}

void *produce(void *param) {
    buffer_item rand;

    while (1) {
        /* sleep for a random period of time */
        sleep(1);

        /* generate a random number */ 
        rand = rand_r(5);
        if (insert_item(rand) < 0) printf("Error");
    }
}

void *consumer(void *param) {
    buffer_item rand;
    while (1)
    {
        /* sleep for a randmo periof of time*/
        sleep(1);
        if (remove_item(&rand) < 0) printf("Error");
        /* code */
    }
}

int main(int argc,char* argv[]) {
    int i; // producer  count 
    int j; // consumer count
    /* get command line arguments argv[1-3] */
    if (argc != 3) {
        printf("Error you need 3 arguments ");
        return 1;
    }
    /* initialiez buffers mutex semaphore */
    pthread_t producer[BUFFER_SIZE];
    pthread_t consumer[BUFFER_SIZE];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    /* create producer threads */
    for (i = 0; i < BUFFER_SIZE; i++) {
        if (pthread_create(&producer[i], NULL, &insert_item, NULL) != 0) {
            printf("pthread producer %d failed to initialize\n", i);
            return 1;
        }
    }

    // producer thread join
    for (i = 0; i < BUFFER_SIZE; i++) {
        if (pthread_join(producer[i], NULL) != ) {
            printf("pthread producer %d failed to join\n", i);
            return 2;
        }
    }

    /* create consumer threads */
    for (j = 0; j < BUFFER_SIZE; j++) {
        if (pthread_create(&consumer[j], NULL, &remove_item, NULL) != 0) {
            printf("pthread consumer %d failed to initialize", j);
            return 3;
        }
    }

    // consumer thread join
    for (j = 0; j < BUFFER_SIZE; j++) {
        if (pthread_join(consumer, NULL) != 0) {
            printf("pthread consumer %d failed to join", j);
            return 4;
        }
    }

    /* sleep */
    sleep(1);

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
    return 0;
}