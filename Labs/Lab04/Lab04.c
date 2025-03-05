#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>


 // check now if this orig number is a perfect one
    /* 
    Example -> 50

    Step 1 -> take square root of the orig number sqrt(orig_number)
    1,50
    2,25
    3,16
    4,12
    5,10
    6,8
    7,7

    Step 2 -> now you need to add all of them together from 2 to 7
    That can work if we do the following;
    orig_number/2 -> 25 for this example
    summation += (orig_number/i)
    then check if the summation is equal to the orig_numebr.
    if so then is perfect number. 
    
    but how can I make each thread works with each 'i' ?
    how can I assign a thread per i?
    */

// find if the given number is perfect number

// global var, and, locking mutex for preventing race conditions
int sum = 0;
pthread_mutex_t mutex;
int N;
int P;


// allocating data of the threads
// struct is used for simplification by bundeling the pieces of the data toegher into one single object
typedef struct {
    int start;
    int end;
} dataThread;
void* summation(void* arg) {
    // alloocating memory for the dataThread
    dataThread* data = (dataThread*)arg;
    int start = (*data).start; // equivalinet data->start
    int end = (*data).end;

    // Looping through each assigned range finding factors of N
    for (int i = start; i <= end; i++) {
        if (N % i == 0) { // i is factor of N 
            // locking 
            pthread_mutex_lock(&mutex);
            sum += i;
            if (i != N / i && i != 1) { // avoiding duplicating factors
                sum += N / i;
            }
            // unlocking
            pthread_mutex_unlock(&mutex);
        }
    }
    free(data); // the allocated memory need to be freed for the dataThread
    return NULL;
}

    // Each thread needs to know the part of the range that is responsible
    // Considering that the distribution will not always be equal:
    // work_per_thread = N / P
    // threads_working_extra_1 = N % P


    // Meaning that 7 elements and 3 threads.
    /*
    7/3 = 2
    7%3 = 1

    if threads_workign_extra_1 > 0 {
    
    }
    2/6+2/6+2/6
    */
    // for (int i = 2; i < P; i++) {
    // i = 2 is incorrecct because you are excluding 2 threads

    // for (int i = 0; i < P; i++) {
    //     // we need a range for each thread

    //     int start = 2 + (i * elements_per_thread); // starts at 2 because we exclude 1,N/1 -> the first factorizations
    //     int end = start + elements_per_thread - 1;

    //     // Threads handling extra elements condition
    //     if (threads_extra_element > i) {
    //         end += 1;
    //     }

    //     // calculating all the elements per thread
    //     for (int j = start; j <= end; j++) {
    //         if (number % j == 0) { // j factor of N?
    //             // mutex lock
    //             pthread_mutex_lock(&mutex);
    //             sum += j;
    //             // mutex unlcok
    //             pthread_mutex_unlock(&mutex);
    //         }
    //     }        
    // }



int main(int argc, char* argv[]) {
    // check the correct number of arguments!
    if (argc < 3) {
        printf("Use: %s <number> <threads>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    P = atoi(argv[2]);
    int number = (int)sqrt(N);
    // if (P != (int)sqrt(number)) {
    //     printf("Please ensure that number of threads is equal to the square root of %d.\n", number);
    // }

    // I wanted to make number of threads equal to the square root of the number that I am trying to analyze that is perfect numebr
    // But threads can work and be responsible for multiple chunks of the factor list
    // With fewer threads we can divide the work across those threads. (pretty interesting)
    if (P > number) {
        printf("Please ensure that number of threads is equal(&& > 0) to the square root of %d.\n", number);
    }
    pthread_t th[P];

    // Initialization of the mutex
    pthread_mutex_init(&mutex, NULL);

    // calculating all the ranges that each thread will work on.
    int step = number / P + 1;
    for (int i = 0; i < P; i++) {
        // allocating memory for the datathread
        dataThread* data = (dataThread*)malloc(sizeof(dataThread));
        (*data).start = i * step + 1;
        (*data).end = (i + 1) * step;

        // Adjust end range if exceed sqrt(N)
        if ((*data).end > number) {
            (*data).end = number;
        }
        // create thread passing 'data' as argument
        if (pthread_create(&th[i], NULL, &summation, data) != 0) {
            perror("Error creating thread");
            return i + 1;
        } 
    }

    // wait all threads to complete
    for (int i = 0; i < P; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Error with thread join.");
            return i + 1;
        }
    }
    // mutex destory
    pthread_mutex_destroy(&mutex);

    // check if N is perfect number.
    if (sum == N) {
        printf("%d is a perfect number!\n", N);
    } else {
        printf("%d is not a perfect number\n", N);
    }

    return 0;
}

    // // Creation of threads
    // for (int i = 0; i < P; i++) {
    //     if (pthread_create(&th[i], NULL, &summation, NULL) != 0) {
    //         perror("Thread had an issue.");
    //         return i + 1;
    //     }
    // }
    // // join of threads
    // for (int i = 0; i < P; i++) {
    //     if (pthread_join(th[i], NULL) != 0) {
    //         perror("Thread had an issue");
    //         return i + 1;
    //     }
    // }

    // // mutex destroy
    // pthread_mutex_destroy(&mutex);
    // if (sum == N) {
    //     printf("%d is perfect number!", N);
    // } else {
    //     printf("%d is not perfect number!", N);
    // }
