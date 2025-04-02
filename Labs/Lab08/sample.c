// Issues with the code:
/*
    Has multiple threads = 50
    Each thread generates random points (x,y) and check if they fall inside the circle
    g_PI is shared -> concurrent updates from multiple threads can lead to race condtion
    it will lead to unprecise answer

    mutex lock is needed to make sure threads are updateing one at a time
    lock must be before modyfing g_PI then release afterward
*/

#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <time.h>

pthread_mutex_t pi_mutex;

typedef struct{
  int arr_len;
} args_struct;

double g_PI = 0.0;


void* monte_carlo(void* v_as) {
    args_struct* as = v_as;
    int arr_len     = (*as).arr_len;
    for (int i = 0; i < arr_len; i++) {
        unsigned int seed = time(NULL) ^ pthread_self();
        double random_x = rand_r(&seed) / (double)RAND_MAX;
        double random_y = rand_r(&seed) / (double)RAND_MAX;
        local_count += (random_x*random_x + random_y*random_y <= 1);

    }
    // locking before modifyign pi
    // unlocking after modifyign pi
    pthread_mutex_lock(&pi_mutex);
    g_PI += local_count;
    pthread_mutex_unlock(&pi_mutex);

    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    int i;
    void* v_args;

    args_struct* args = malloc(sizeof(args_struct));
    double pi = 0;
    int num_threads = 50;
    pthread_t thread[num_threads];
    srand(time(NULL));

    if (argc != 2) {
        printf("ERROR: Needs exactly 1 argument.\n");
        exit(1);
    }
    int len = atoi(argv[1]);

    (*args).arr_len = len;
    v_args = (void*)(args);

    // initializing mutex
    pthread_mutex_init(&pi_mutex, NULL);

    for (i = 0; i < num_threads; i++) {
        if (pthread_create(&thread[i], NULL, &monte_carlo, v_args) != 0) {
            printf("ERROR: Monte Carlo thread created incorrectly.\n");
            exit(0);
        }
    }

    for (i = 0; i < num_threads; i++)
        pthread_join(thread[i], NULL);

    pi = g_PI*4.0/(len * num_threads);

    printf("Pi is: %f\n", pi);
    free(args);
    // destorying mutex
    pthread_mutex_destroy(&pi_mutex);
    return 0;
}

