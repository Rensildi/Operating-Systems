/*  

    Create an integer array num_array consisting of the following elements
    {1,2,3,4,5} - Creates a child process using fork.

    Check and managed for failed fork condition

    Withing the parent process, for each entry in num_array[],
    square the entry input and add that to summation variable.

    Within the child process, for each entry in num_array[],
    multiply the entry input by two and add that to summation variable.

    After the parent and child conditions are completed,
    output the value of sum and print out each element in num_array
    for both processes using one set of commands.abort

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()
#include <sys/wait.h> // for wait()

int main(int argc, char* argv[]) {
    // integer array

    // each process has an id
    // the child processes created by fork() have a default id of '0'
    // the main process doesn't
    int process_id = fork(); //pid
    int num_array[] = {1,2,3,4,5};
    
    // Aparently sizeof returns the total size in BYTES
    // not the number of elements
    // To calculate the number of elements in the array is:
    // number of elements = total size of array / size of one element
    int length = sizeof(num_array) / sizeof(num_array[0]); 


    int child_sum = 0; // the summation of the child processor
    int parent_sum = 0;; // the summation of the parent processor


    if (process_id < 0) {
        perror("Cannot find the location of the child processr.\n The PID should not be negative.");
        return -1;
    }

    if (process_id == 0) {
        // child process -> multiply by 2
        printf("Child Process Summation:\n");
        for (int i = 0; i < length; i++) {
            num_array[i] = num_array[i] * 2;
            child_sum += num_array[i];
            printf("num_array[%d] = %d\n", i, num_array[i]);
        }
        printf("Sum = %d\n\n", child_sum);
    } else {
        // parent process ->  square the element
        wait(NULL);
        printf("Parent Process Summation:\n");
        for (int i = 0; i < length; i++) {
            num_array[i] = num_array[i] * num_array[i];
            parent_sum += num_array[i];
            printf("num_array[%d] = %d\n", i, num_array[i]);
        }
        printf("Sum = %d\n\n", parent_sum);
    }

    // issues with the wait() if I try to print the sum here.
    //printf("Summation of child = %d\nSummation of parent = %d\n", child_sum, parent_sum);
    
    

    return 0;
    
}