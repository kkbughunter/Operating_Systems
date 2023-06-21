#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 100

float avg;
int min, max;

void *average(void *inp)
{
	int sum = 0, *vals = (int *)inp;
	
	for (int i=1; i<vals[0]; i++)
		sum += vals[i];
		
	avg = (float)sum / (vals[0]-1);
	pthread_exit(0);
}


void *minimum(void *inp)
{
	int *vals = (int *)inp;
	min = vals[1];
	
	for (int i=1; i<vals[0]; i++)
		if (vals[i] < min)
			min = vals[i];
	pthread_exit(0);	
}


void *maximum(void *inp)
{
	int *vals = (int *)inp;
	max = vals[1];
	
	for (int i=1; i<vals[0]; i++)
		if (vals[i] > max)
			max = vals[i];
	pthread_exit(0);	
}


int main(int argc, char *argv[])
{
	pthread_t avg_thread, min_thread, max_thread; /* the thread identifier */
	pthread_attr_t attr; /* set of thread attributes */
	if (argc == 1)
	{
		fprintf(stderr,"Enter values for calculation!\n");
		return -1;
	}
	
	int *vals = (int *) malloc(SIZE * sizeof(int));
	vals[0] = argc;
	printf("\nNumbers to be passed to threads : ");
	for (int i = 1; i < argc; i++)
	{
		vals[i]= atoi(argv[i]);
		printf("%d ", vals[i]);
	}
	printf("\n\n");
	
	/* initialise the default attributes */
	pthread_attr_init(&attr);
	
	pthread_create(&avg_thread, &attr, average, vals);
	pthread_create(&min_thread, &attr, minimum, vals);
	pthread_create(&max_thread, &attr, maximum, vals);
	
	/* wait for the thread to exit */
	pthread_join(avg_thread,NULL);
	pthread_join(min_thread,NULL);
	pthread_join(max_thread,NULL);
	
	printf("Average value from thread 1 = %.3f\n", avg);
	printf("Minimum value from thread 2 = %d\n", min);
	printf("Maximum value from thread 3 = %d\n", max);
}

