#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>	
#include <stdbool.h>

#define MAX_SIZE 1001
#define QUEUE_SIZE 5

typedef char ItemType;


ItemType queue[QUEUE_SIZE];
ItemType consumer_arr1[MAX_SIZE];
ItemType consumer_arr2[MAX_SIZE];
int ind1,ind2;
FILE* fp;
int in=0,out=0;
pthread_mutex_t mutex;
sem_t full,empty;
ItemType ch;
int input_size;
bool continue_prod = true;
bool continue_cons = true;
bool continue_cons2 = true;

void* produce(void* args){

	while(continue_prod)
	{
		sem_wait (&empty);
		pthread_mutex_lock (&mutex);

		if(fscanf(fp," %c" , &ch) == EOF){
			continue_prod = false;
		}else{

			queue[in] = ch;
			#ifdef DEBUG
				printf("PRODUCER   %c\n",ch);
			#endif
		//	input_size ++;
			
			in = (in+1) % QUEUE_SIZE;
		}
		pthread_mutex_unlock(&mutex);
		sem_post (&full);
		usleep(100000);
	}

	return NULL;
}
void* consume1 (void* args){
	while(continue_cons){
		sem_wait(&full);
		pthread_mutex_lock (&mutex);
		#ifdef DEBUG
			printf("CONSUMER-1 %c \n",queue[out]);
		#endif
		
		consumer_arr1[ind1] = queue[out];
		ind1 = ind1 + 1;
		
		out = (out+1) % QUEUE_SIZE;

		if(3*ind1 >= (2*input_size)){

			continue_cons = false;
		}
		pthread_mutex_unlock (&mutex);
		sem_post (&empty);
		
		usleep(50000);

	}
}

void* consume2 (void* args){
	while(continue_cons2){
		usleep(100800);
		sem_wait(&full);
		pthread_mutex_lock (&mutex);
		#ifdef DEBUG
			printf("CONSUMER-2 %c \n",queue[out]);
		#endif
		
		consumer_arr2[ind2] = queue[out];
		ind2 = ind2 + 1;		

		out = (out+1) % QUEUE_SIZE;

		if(ind2 >= input_size/3){
			continue_cons2 = false;
		}
		
		pthread_mutex_unlock (&mutex);
		sem_post (&empty);
		usleep(100000);
	
	}
}


int main(int argc, char const *argv[])
{
	int i;
	printf("Enter the number of characters in the file\n");
	scanf("%d",&input_size);

	fp = fopen("string.txt","r");
	
	assert(fp != NULL);

	#ifndef DEBUG
		printf("Threads running...\n\n");
	#endif

	ind1 = ind2 = 0;

	sem_init(&full,0,0);
	sem_init(&empty,0,QUEUE_SIZE);

	pthread_mutex_init(&mutex, NULL);


	pthread_t prod,cons1,cons2;

	pthread_mutex_init (&mutex,NULL);	//use default


	pthread_create (&prod,NULL,produce,NULL);
	pthread_create (&cons1,	NULL,consume1,NULL);
	pthread_create (&cons2,	NULL,consume2,NULL);


	pthread_join(prod,NULL);
	pthread_join(cons1,NULL);
	pthread_join(cons2,NULL);


	printf("1st consumer thread: \n");
	for (i = 0; i < ind1; ++i)
	{
		printf("%c ",consumer_arr1[i]);
	}
	printf("\n");

	printf("2nd consumer thread\n");

	for (i = 0; i < ind2; ++i)
	{
		printf("%c ",consumer_arr2[i]);
	}
	printf("\n");

	return 0;
}
