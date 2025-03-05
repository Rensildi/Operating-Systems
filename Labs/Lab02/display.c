/*           Task 2               */

// m start, n end, fileNmae, df
// open()
#include <fcntl.h>  // open()
#include <unistd.h> // close(), read(), write()
#include <errno.h>  // error handling
#include <stdio.h>  // printf(), perror()
#include <stdlib.h> // atoi()
#include <string.h> // strerror()
#define BUFFER_SIZE 128

// ./show --> the program is being created
// Lab02.txt argument passed in the program

//  int open(const char *argv[1], int flags);

int main(int argc, char *argv[]) {
    // argc == 2
    // argv[0] == ./show
    // argv[1] == Lab02.txt

    if (argc != 5) {
    printf("Program is expecting four arguments: (m) (n) (fileName) (destinationFile)\n");
    return 1;
    }


    // command line arguments are always strings
    // therefore m and n are strings as well e.g., '2', '5' ... 
    // so we need to convert m and n to integers
    // atoi() pg.58 
    // char m_str[] = argv[1];
    // char n_str[] = argv[2];
    
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);

    if (m > n || m < 1 || n < 1) {
        printf("You are trying to substract more (m) than the total (n).\n");
        return 1;
    }


    int fd_fileName = open(argv[3], O_RDONLY);
    if (fd_fileName == -1) {
        perror("Cannot open the file. Check if the file is in the directory!");
        return 1;
    }
    
    int fd_desitnationFile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644); //0644 == rw-r--r--
    if (fd_desitnationFile == -1) {
        perror("Cannot open destination file.");
        close(fd_fileName);
        return 1;
    }



    // holding the data read from file
    int bytes_read = 0;
    char buffer[BUFFER_SIZE];

    // keep count for all the lines until m
    int count = 0;

    // bytes_read = read(fd, buffer, BUFFER_SIZE);

    while ((bytes_read = read(fd_fileName, buffer, sizeof(buffer)))) {

        for (int i = 0; i < bytes_read; i++) {
            // When starting a new line keep count of the line(i's)
            if (buffer[i] == '\n') {
                count++;
            }
            // write to destination files all the lines after m
            if (count >= m && count <= n + 1) {
                if (write(fd_desitnationFile, &buffer[i], 1) == -1) {
                    perror("Cannot write destination file");
                    close(fd_fileName);
                    close(fd_desitnationFile);
                    return 1;
                }
            }

            // Stop reading
            if (count > n) {
                break;
            }
        }

        if (count > n) {
            break;
        }
        // buffer[bytes_read] = '\0'; // null-terminate helps printf to find where to end.
        // printf("%s\n", buffer);
        // is the same as read(fd, buffer, BUFFER_SIZE)) > 0
        // if (bytes_read == 0) {
        //     printf("You have reached the end of the line.");
        //     close(fd);
        //     return -1;
        // }
    }
    if (bytes_read == -1) {
        perror("Error reading!");
        return -1;
    }
    close(fd_fileName);
    close(fd_desitnationFile);
    return 0;
}



// /*           TASK 1               */
// // open()
// #include <fcntl.h> // open()
// #include <unistd.h> // close()
// #include <errno.h> // erro handling
// #include <string.h> // strerror()
// #include <stdio.h> // printf and perror
// #define BUFFER_SIZE 128

// // ./show --> the program is being created
// // Lab02.txt argument passed in the program

// //  int open(const char *argv[1], int flags);

// int main(int argc, char *argv[]) {
//     // argc == 2
//     // argv[0] == ./show
//     // argv[1] == Lab02.txt

//     if (argc != 2) {
//         printf("Program is expecting one argument\n");
//         return 1;
//     }

//     // char location[] = "ComputerOperatingSystemsLab/Labs/Lab02/Lab02.txt";
//     int fd = open(argv[1], O_RDONLY); //location == argv[1] == Lab02.txt
//     if (fd == -1) { // if file fails to open
//         // if there are errors in opening the file
//         perror("Error opening file\n");
//         return 1;
//     }

//     // holding the data read from file
//     int bytes_read = 0;
//     char buffer[BUFFER_SIZE];

//     // bytes_read = read(fd, buffer, BUFFER_SIZE);

//     while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0 ) {
//         buffer[bytes_read] = '\0'; // null-terminate helps printf to find where to end.
//         printf("%s\n", buffer);
//         // is the same as read(fd, buffer, BUFFER_SIZE)) > 0
//         // if (bytes_read == 0) {
//         //     printf("You have reached the end of the line.");
//         //     close(fd);
//         //     return -1;
//         // }
//     }
//     if (bytes_read == -1) {
//         perror("Error reading!");
//         close(fd);
//         return -1;
//     }
//     printf("You have reached the end of the line.");
//     close(fd);
//     return 0;
// }