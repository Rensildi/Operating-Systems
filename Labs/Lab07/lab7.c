#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NO_OF_QUEUE       20	/* #of Students */
#define NO_OF_SLOTS       5

int     Q[NO_OF_SLOTS];
int     front = -1;
int     rear  = -1;

pthread_mutex_t mutex;

void *Supervisor(void *arg){
	int student = (int)arg;

	rear = rear + 1;
	if (rear >= NO_OF_SLOTS) rear = 0;
	Q[rear] = student;

	printf("%4d. Student was called to the Presentation\n", student+1);

}

void *Student(void *arg){
	int student = 0;
    	int studentsRemoved = 0;

	printf("Presentation  is started\n");

	while (studentsRemoved < NO_OF_QUEUE){

		front = front + 1;
		if (front >= NO_OF_SLOTS)	front = 0;
		student = Q[front];
		printf(" %4d. Student is done. It was removed from the Queue \n", student+1);
		studentsRemoved++;

		usleep((int)(drand48()*500000));
	}
}

int main(int argc,char *argv[]){
	int i;
	pthread_t supervisorId;
	pthread_t QUEUEId;
	
	srand48(time(NULL));

	/* supervisor thread */
	pthread_create(&QUEUEId, NULL, Student, NULL);

	for(i=0; i < NO_OF_QUEUE;i++){
		pthread_create(&supervisorId, NULL, Supervisor, (void *)i);
	}

	pthread_join(QUEUEId, NULL);

	return 0;
}
