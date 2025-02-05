#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 128 //adjust depending on what you want to input in the datastructures.
// microscale is good practice for the buffersize
int main(int argc, char *argv[]) {
    argc = number of paramters at execution
    argv[] = stores the array of strings of such arguments.

    // file descriptor == pointer to the file
    // identifier for the call reference throught the system calls

    char location[] = "/home/rkala/Downloads/file" //locatoin
    int fd = open("myfile_location", O_RDONLY | O_WRONLY);
    

    // READ & WRITE opeartions
    int bytes_read = 0; // can stay within the boundaries of the contents we want in the datastructure.
    char buffer[128];
    // read from locatoin -> put the contet into datastructure -> check the maximum of elements that are supposed to be in datastructure
    bytes_read = read(fd, buffer, max_size_that_buffercanfit);
    // 300 elements/bytes we are reading.
    while(bytes_read = read(fd, buffer, BUFFER_SIZE)) {
        // do whatever work we have...
        // doing work on buffer[0] to buffer[127]
        // put work output into output_str
        char output_str[128]
        // if there were only 50 elements in it and you had a static buffersize, then it will read those 50elemnts plus 128-50
        // It will read out of that memory bounds.
        write(fd2, output_str, sizeof())
    }
}