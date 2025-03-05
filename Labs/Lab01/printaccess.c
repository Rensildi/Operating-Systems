#include <stdio.h>
#include <time.h> // Library that invludes functions to handle format time data.
#include <sys/stat.h> // Provides the stat structure and functions to retrive file info.
int main (int argc, char *argv[])
{
// define an instance of struct stat (don’t use a pointer). --Store the file information
// struct stat holds file attributes like size/permissions/timestamps
struct stat filestats;
// Use the stat function store the status of the file in your stat struct
// stat takes a pointer, so pass your struct by reference with &
printf("Checking file: %s\n", argv[1]);

// fills the filestats struct with info from file in argv[1]
if (stat(argv[1], &filestats) == -1)
{
    // use perror to print error message then exit program
    perror("Failed: The file may not exist or there is an issue to access it!");
    return 1;

}
    // print the last access time for the file using defined instance of
    // struct stat. ctime requires a pointer, hence the & below.

    // ctime() converts the time_t value to a readable string
    printf ("%s last accessed at %s", argv[1], ctime(&filestats.st_atime)); // when was last time the file was accssessed
    
    return 0;
}