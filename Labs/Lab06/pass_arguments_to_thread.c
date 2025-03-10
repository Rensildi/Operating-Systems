#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// // Problem
// /*
//     Create 10 threads, each taking a unique prime
//     from the primes array and print it on the screen.
// */

// int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


// void* routine(void* arg) {
//     int index = *(int*)arg;
//     printf("%d ", primes[index]);
//     free(arg);
// }
// int main(int argc, char* argc[]) {
//     pthread_t th[10];
//     for (int i = 0; i < 10; i++) {
//         int* a = malloc(sizeof(int));
//         *a = i;
//         if (pthread_create(&th[i], NULL, &routine, a) != 0) {
//             perror("Failed to create thread.");
//             return 1;
//         }
//     }

//     for (int i = 0; i < 10; i++) {
//         if (pthread_join(th[i], NULL) != 0) {
//             perror("Failed to join thread.");
//             return 2;
//         }
//     }
//     return 0;
// }


// Problem
/*
    Create 2 threads, then take the sum of all prime numbers
*/

int primes[10] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};


void* routine(void* arg) {
    int index = *(int*)arg;
    int sum = 0;
    for (int j = 0; j < 5; i++) {
        sum += primes[index + j];
    }
    printf("Local Sum: %d\n", sum);
    *(int*)arg = sum;
    return arg;
}
int main(int argc, char* argc[]) {
    pthread_t th[2];
    for (int i = 0; i < 2; i++) {
        int* a = malloc(sizeof(int));
        *a = i * 5;
        if (pthread_create(&th[i], NULL, &routine, a) != 0) {
            perror("Failed to create thread.");
            return 1;
        }
    }

    int globalSum = 0;
    for (int i = 0; i < 2; i++) {
        int* r;
        if (pthread_join(th[i], (void**) &r) != 0) {
            perror("Failed to join thread.");
            return 2;
        }
        globalSum += *r;
        free(r);
    }
    printf("Gloabl sum: %d\n", globalSum);
    return 0;
}