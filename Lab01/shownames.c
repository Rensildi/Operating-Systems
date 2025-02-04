#include <dirent.h> // work with directory streams.
#include <errno.h> // reporting system errors
#include <stdio.h> // standard input output functions.

// argc (argument count) = number of arguments passed to the program)
// argv (argument vector) = array of argument strings
// argv[0] (program name)
// argv[1] (directory name provided by user )

// 

int main (int argc, char *argv[])
{
	struct dirent *direntp;  // represent a directory entry (details about name of the file/directory)
	// define a pointer with type directory stream (DIR):
	DIR *directory; // Directory stream 

	// Check the number of arguments (argc). It should be two. If it
	//   is not two, then print an error message and exit the program:
	if (argc != 2) // check the number of argument
	{ 
		fprintf (stderr, "Usage: %s directory name \n", argv[0]); 
		return 1;
	}

	// The following lines should open the directory given by
	//   argument argv[1] (by using opendir). 

	// Store it in your defined directory stream variable.
	directory = opendir(argv[1]); // open directory that is specified by argv[1] (name of directory)
	// The rest checks if the system call returned a proper value
	//   if not an error message is printed and the program closed
	if (directory == NULL) 
	{ 
		perror ( "Failed to open directory" );
		return 1;
	}


	// Read all the entries in this directory and store them in
	// direntp. readdir will read one entry at a time and increment
	// automatically. This is why it is in a while loop. 
	// Then, print all the file names (using the struct from readdir).

	// readdir(directory) --> reads the next entry in the directory (autoincrements?)
	// readdir(directory) --> then returns pointer to dirent struct
	while ((direntp = readdir(directory)) != NULL) {
		printf ("%s \n" , (*direntp).d_name); // *direntp access the d_name (containing the name of the directory)
		direntp = readdir(directory);
	}

	// close the defined directory stream.

	// closedir(directory) -> closes the directory stream
	
	while ( ( closedir (directory) == -1) && ( errno == EINTR) ); 

	return 0;
}