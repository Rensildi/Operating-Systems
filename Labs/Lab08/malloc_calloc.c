#include <stdio.h>
#include <stdlib.h>

int main(){
  int arr_size = 10;
  int integer_array[arr_size];
  int* malloc_array = malloc(arr_size * sizeof(int));
  int* calloc_array = calloc(arr_size, sizeof(int));

  //malloc does not initialize the memory to zero
  //recycles previously allocated & freed memory, or requests new page(s) from OS
  //may be zero, may also be anything else before written into.
  printf("malloc before fill:\n");
  for (int i=0; i<arr_size; i++)
    printf("malloc_array[%d] = %d\n", i, malloc_array[i]);

  printf("\nmalloc after fill:\n");
  for (int i=0; i<arr_size; i++){
    malloc_array[i] = i;
    printf("malloc_array[%d] = %d\n", i, malloc_array[i]);
  }

  free(malloc_array);
  printf("\nfreed malloc_array, now malloc array2 no fill:\n");
  int* malloc_array2 = malloc(arr_size * sizeof(int));
  for (int i=0; i<arr_size; i++)
    printf("malloc_array2[%d] = %d\n", i, malloc_array2[i]);

  //calloc initializes the memory to zero.
  //same as malloc + memset(0)
  printf("\ncalloc_array after calloc()\n");
  for (int i=0; i<arr_size; i++)
    printf("calloc_array[%d] = %d\n", i, calloc_array[i]);

  //an array is collection of similar data types, pointer variable stores the address of another pointer.
  //int[] stores collection of integers, int * stores address of integer
  int array[arr_size];
  int *arrayp = malloc(arr_size * sizeof(int));
  printf("\nsizeof int array[] (bytes) = %ld\n", sizeof(array));
  printf("sizeof int array[] (elements) = %ld\n", sizeof(array) / sizeof(int));
  printf("sizeof int *arrayp (bytes) = %ld\n", sizeof(arrayp));
  printf("sizeof int *arrayp (num ptrs) = %ld\n", sizeof(arrayp) / sizeof(int*));
  free(arrayp);

  //if we want to reallocate / dynamically change memory allocation, we use realloc
  int new_arr_size = 5;
  malloc_array = realloc(malloc_array, new_arr_size);
  calloc_array = realloc(calloc_array, new_arr_size);

  //free command used to dynamically de-allocate memory
  //free(malloc_array2);
  //free(calloc_array);
}
