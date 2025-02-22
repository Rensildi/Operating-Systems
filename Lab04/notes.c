processor is an executable executing program
a thread is a piece of code within a process

memory
proc completely independednt of one another, deepcopy mem/no shared mem space
- threads *within a process* share the same memory space

thread - lightweight, rpc heavier weight

a proc starts with one thread
a proc can create more threads all threads share process virtual address space

proc shared memory makes communication easier
threads allow a program to perform multiple tasks simultaneously

pthread_create()
pthread_join()
pthread_exit()
pthread_wait()

void func(int pos) {
    // defined as a subroutine
}

int main() {

// create one thread
    pthread_t *thread;
    int s;
    s = pthread_create(&thread, NULL, func, &arg);

    for loop create four threads thread[0] to thread[3] to create three threads
    
        pthread_create(thread1, NULL, func, 0);
        pthread_create(thread2, NULL, func, 1);
        pthread_create(thread3, NULL, func, 2);
        pthread_create(thread4, NULL, func, 3);
        0-25 t0 works on
        26-50 t1 works on 
        51-75 t2 works on

        from (val*25) to (val+1) * 25


        pthread_join() -> 
        effectively tells your process not to continuolly work 
        until all the 4 sections are done. 2d array is completley 
        updated (aka all threads closed out),then work with that
        new data structure iterate thorugh, output, transform,
        whatever...
        (but you need all the processes to complete their work 
        "have transformation or updated peicec of information.")

        // initiezlie the threads tell them to do their work
        // it will continuolly to update the work
        // then when all threads returns complete,
        // the "barrier/wait" stops and then continue with
        // single process
        for i = 0 to num threads 
            pthread_join(thread[i])
        // prevents updates if the threads are not finished executing
        // because it updates before finishing it can lead to inconsistency.
}


////// Lab04

what is a perfect number
if i had number 28
2,14
4,7


/* trick */ 
1,28
2, 14
4, 7

do computations finding gcd from this range
sqrt() -lm library #include <math.h>
gcc rpthread_code.c -o run -lm -lpthread // need to link the pthread library
sqrt()
is 36 a perfect number
sqrt(36) == 6 --> gives the splitting point

if ====> 5.2
the range of characters to find gcd perfect number summation
1-5

1, 36
2, 18 -thread1 check first quarter range
3, 12 -t2 do q2
4,9 -t3 do q3
6,6 -t4 do q4
9,4
12,3
2,18

7,4,
14, 2
28, 1

summation(gcd values)

IF THAT EQUALS THE ORIGINAL NUMEBR , its a perfect number!

27 != 28
thus 
printf(not perfect!)


1-30/50
is 1000 a perfect num
t1 do checks to add to common memory summation variable()
t1 add to summation range from 2-6 from gcd two vars
t2 add summation range from 7-12 from gcd with lower end num being
7-12.

