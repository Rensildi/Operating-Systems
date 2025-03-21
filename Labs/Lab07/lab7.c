#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
// using semaphore
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NO_OF_QUEUE       20	/* #of Students */
#define NO_OF_SLOTS       5

int     Q[NO_OF_SLOTS]; // int array (Q) size 5
int     front = 0;
int     rear  = -1;
int 	count = 0; // replaced studentRemoved with count

pthread_mutex_t mutex;
// initialize a 'sempahore' 
sem_t semaphore; // to keep track of students who are presenting

void *Supervisor(void *arg){
	// initializing student
	int student = *(int*)arg;
	free(arg);

	while (1) {
	// rear start with -1 + 1 = 0, but since there is a loop for threads
	// it will keep incrementing.
	pthread_mutex_lock(&mutex);
	if (count < NO_OF_SLOTS) {
		rear = (rear + 1) % NO_OF_SLOTS;
		Q[rear] = student;
		count++;
		printf("%4d. Student was called to the Presentation\n", student + 1);
		pthread_mutex_unlock(&mutex);
		sem_post(&semaphore);
		break;
	}
	pthread_mutex_unlock(&mutex);
	usleep(10000);
	}
	return NULL;
}

void *Student(void *arg){
	int student = 0;
	printf("Presentation started\n");

	// while studentsRemoved (initialy set as 0) < 20
	// runs until all students are removed from the queue
	for(int i = 0; i < NO_OF_QUEUE; i++) {
		sem_wait(&semaphore);
		pthread_mutex_lock(&mutex);

		student = Q[front];
		front = (front + 1) % NO_OF_SLOTS;
		count--;

		printf(" %4d. Student is done. It was removed from the Queue \n", student + 1);

		pthread_mutex_unlock(&mutex);
		usleep((int)(drand48() * 500000));
	}
	return NULL;
}

int main(int argc,char *argv[]){
	// adding multiple (5) threads for supervisorId
	pthread_t supervisorId[NO_OF_QUEUE];
	pthread_t QUEUEId;
	
	srand48(time(NULL));

	// initializing semaphore and mutes
	pthread_mutex_init(&mutex, NULL);
	// there should not be any student to be done without starting.
	sem_init(&semaphore, 0, 0);

	/* supervisor thread */
	pthread_create(&QUEUEId, NULL, Student, NULL);

	for(int i=0; i < NO_OF_QUEUE;i++){
		int *arg = malloc(sizeof(int));
		*arg = i;
		pthread_create(&supervisorId[i], NULL, Supervisor, arg);
	}

	for (int i = 0; i < NO_OF_QUEUE; i++) {
		pthread_join(supervisorId[i], NULL);
	}

	pthread_join(QUEUEId, NULL);

	// initializing the destructors
	pthread_mutex_destroy(&mutex);
	sem_destroy(&semaphore);

	return 0;
}