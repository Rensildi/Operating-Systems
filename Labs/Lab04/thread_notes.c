//////////////////// THREAD CREATION //////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

// when create a thread it execute a function
void* routine() {
    // showing that both threads are working in parallel
    /* the output should look like this
    CORRECT output expectation
    Test from threads
    Test from threads
    Ending thread
    Ending thread

    WRONG output expectation
    Test from threads
    Ending thread
    Test from threads
    Ending thread
    */
    printf("Test from threads\n");
    sleep(3);
    printf("Ending thread\n");
}

// To start creating a thread we have to start creating a place (variable)
// The API will store some information about the thread

int main(int argc, char* argv[]) {
    pthread_t t1, t2;
    // parameter = pointer of pthread_t "t1" variable created at line 16 so simply pass its refernce "&"
    // attributes = customizations of certain things dealing with threads (since is a pointer you can pass NULL which will set it up itself with default attribute)
    // function = It is going to execute, but to pass the function as parameter you need a reference '&'
    // argument = it is going to be passed to the routine if you have nothing pass NULL
    // pthread_create(&parameter,attribute,&fucntion,argument)
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);
    // we have to wait for the thread to finish its execution.
    // if we dont it will terminate the process all together because inconsistency is created.

    // structure/struct = t1 which holds the information of that thread
    // result_pointer = pointer that gets the result from the thread (if not using it then pass NULL)
    // pthread_join(structure/struct, result_pointer);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

//////////////// MUTEX INITIALIZATION /////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>


int mails = 0;
pthread_mutex_t mutex;

void* routine() {
    for (int i = 0; i < 100000000; i++) {
        // Preventing race conditinons by using mutex(lock)
        // keeping track of the exact data is important
        pthread_mutex_lock(&mutext);
        mails++;
        pthread_mutex_unlock(&mutext);

    }
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    // you need to initialize mutex and then destroy.
    pthread_mutex_init(&mutext, NULL);
    if (pthread_crate(&p1, NULL, &routine, NULL) != 0) { 
        return 1;
    }
    if (pthread_join(p1, NULL) != 0 ) {
        return 2;
    }
    pthread_mutex_destroy(&mutext);
    return 0;
}

///////////// Efficient way of thread creation (for loop) //////////////
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

int mails = 0;
pthread_mutex_t mutex;

void* routine() {
    for (int i = 0; i < 100000000; i++) {
        // Preventing race conditinons by using mutex(lock)
        // keeping track of the exact data is important
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);

    }
}

int main(int argc, char* argv[]) {
    // All the threads that are going to be stored
    pthread_t th[4];
    // mutex initialization
    pthread_mutex_init(&mutex, NULL);

    // looping through all the threads
    // do not create and join threads in the same for loop
    // is better practice to use 2 different for loops
    // Example
    /*
    ///////////////////// inside 1 for loop: //////////////
    Thread 1 has started
    Thread 1 has finished
    Thread 2 has started
    Thread 2 has finished
    Thread 3 has started
    Thread 3 has finished
    Thread 4 has started
    Thread 4 has finished

    ////////////////// 2 for loops: ////////////////
    Thread 1 has started
    Thread 2 has started
    Thread 3 has started
    Thread 4 has started
    Thread 1 has finished
    Thread 2 has finished
    Thread 3 has finished
    Thread 4 has finished


    (
    Keeping in mind that the messages that are printed,
     their order does not mean 
    they finished the execution that exact order.
    It is just a way to print the messages.
    We are only waiting for the threads to finished
    Not the way of how they finished the execution. 
    )


    */
    for (int i = 0; i < 4; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread.");
            return 1;
        }
        printf("Thread %d has started\n", i);
    }
    for (int i = 0; i < 4; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            return 2;
        }
        printf("Thread %d has finished execution\n", i);
    }

    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;
}