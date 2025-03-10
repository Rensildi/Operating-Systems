#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

// getting the value generated from roll_dice() to main()
// void* is used to send the result
// pthread_join() will take a reference to the pointer (double pointer) and set the value from within it to what ever rand chooses
void* roll_dice() {
    int value = (rand() % 6) + 1;
    int* result = malloc(sizeof(int));
    *result = value;
    //printf("%d\n", value);
    // returning the reference
    printf("Thread result: %p\n", result);
    return (void*) result;
} 

int main(int argc, char* argv[]) {
    int* res;
    srand(time(NULL));
    pthread_t th;
    if (pthread_create(&th, NULL, &roll_dice, NULL) != 0) {
        return 1;
    }
    // pthread_join() will take a reference to the pointer (double pointer) and set the value from within it to what ever rand chooses
    // if (pthread_join(th, NULL) != 0) {
    // how to get the value from roll_dice()? -> dynamic allocation 
    if (pthread_join(th, &res) != 0) {
        return 2;
    }
    print("Main res: %p\n", res); // checking if we are checkign the correct address.
    printf("Result: %d\n", *res);
    free(res);
    return 0;
}