#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

pthread_t thread[2];
pthread_mutex_t mutex;
pthread_cond_t threshold;
int number = 0, i = 0;

void *threadlock(int thread_num)
{
	pthread_mutex_lock(&mutex);

	if (thread_num == 2 && number == 0){
		
		//pthread_cond_signal(&threshold);
		pthread_cond_wait(&threshold, &mutex);	
	}

	pthread_mutex_unlock(&mutex);

	for (i = 0; i < 10; i++){
		pthread_mutex_lock(&mutex);

		if (thread_num == 1)
			printf("thread 1 : ");

		else if (thread_num == 2)
			printf("thread 2 : ");

		printf("number = %d, i = %d\n", number, i);
		number++;

		pthread_cond_signal(&threshold);

		if (i == 9){
			pthread_mutex_unlock(&mutex);
			break;
		}

		pthread_cond_wait(&threshold, &mutex);

		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

void thread_create(void)
{
	/*int temp;
	memset(&thread, 0, sizeof(thread));*/

	pthread_mutex_lock(&mutex);

	/*if ((pthread_create(&thread[0], NULL, threadlock, 1)) != 0)
		printf("create function : thread 1 create fail\n");

	else
		printf("create function : Thread 1 is established\n");*/

	if ((pthread_create(&thread[1], NULL, threadlock, 2)) != 0)
		printf("create function : thread 2 create fail\n");

	else
		printf("create function : Thread 2 is established\n");

	if ((pthread_create(&thread[0], NULL, threadlock, 1)) != 0)
		printf("create function : thread 1 create fail\n");

	else
		printf("create function : Thread 1 is established\n");

	pthread_mutex_unlock(&mutex);
}

void thread_wait(void)
{
	if (thread[0] != 0){
		pthread_join(thread[0], NULL);
		printf("wait function : Thread 1 over\n");
	}

	if (thread[1] != 0){
		pthread_join(thread[1], NULL);
		printf("wait function : Thread 2 over\n");
	}
}

int main()
{
	pthread_mutex_init(&mutex,NULL);
	printf("main funtion : establishing threads\n");
	thread_create(); 
	printf("main funtion : waiting for threads\n");
	thread_wait(); /* 用pthread_join卡著 */
	printf("main funtion : turn me and exit\n");
	return 0;
}
