/* 02/05/2025 lab Lecture+notes */

// Lab 3 notes

// main funcion for this lab --> fork()

// It creates a subprocess

// You are going to have a process with the certain values and variables.
// Then create a fork
// And modify that array.
// Fork()
// modify the subset
// outpout what is requiring.
ps command -> shows each process running
Example 
voldemort@MSI:/mnt/c/Users/rkala$ ps
    PID TTY          TIME CMD
    281 pts/0    00:00:00 bash
    395 pts/0    00:00:00 ps

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int pid;
    char var = 'A';
    double num = 2.4;

    // you can run 4 forks at the same time

    pid = fork();
    pid = fork();
    pid = fork();
    pid = fork();

    wait(); // for high usage of processes.

    if parent{}
    do this
    if child{} 
    do this

    do subroutine of summation()
    printf("summation = %d\n", summation);

    // do work right here on both parent and child. separate deep copies, both run the same command

    function(var, num);

    if (pid == 0) {
        within child, do work only here
    }
    if pid < 0
        throw some error

    if () > 0 {
        // wait, after the child process closes out, that previous unsuable memory. wait will allow the computer to free that unsuable memory.
        wait()
    }
    were within parent process 

    return 0;
}