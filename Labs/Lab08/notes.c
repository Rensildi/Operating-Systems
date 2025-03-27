// lab08
A = pi(r)^2

we dont konw what pi is

pi = 4 * (number of points in circle)/ (total number of points)

A/4 = pi

figure out where to synchronize it up to the point where what global variable is in here
that we need to surround mutexes, 
and we need to guard that critical section 
So we dont have overhead errors and syncrhonization errors


// Dynamic memory allocation (malloc commands)
malloc
calloc
realloc
free

go into my code, start bulding out structure,
as i need new DS/LL/Nodes, I dynamically allocate them and grow my DS

int array[100];
[1000000]

why use dynamic memory allocations

data, f/b ptrs
trees, graphs, nodes, linked lists
--> flexibility of datastructure size

int array[100];
int *array; // allocating and setting up a pointer variable to a memory location(16bytes maybe)
// dynamically allocate
int *array = (int *)malloc((100+1)*sizeof(int *)) // +1 prevent edge cases that might cause errors that cant find a string
char *array = (char *)malloc((100 +1)*sizeof(char *));

// dynamic allocation downside: malloc and corresponding free to clear out DS, can take more overhead specifically if you add many free
// 
free(array);

calloc --> malloc + clear out/zero out all blocks
[123456789]
looping through and zeroing ourself
[000000000]
// then we have to free just like malloc the block
free()

realloc(DS, size) --> shring/grow our Dynamic Memory data structure


array of strings
char *array[16];
char array[16][20];
for (int i =0; i< 16; i++) {
    array[i] = malloc(20);

}
// left at minute 36:05





