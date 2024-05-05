#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define SIZE 16


typedef struct job // СЃС‚СЂСѓРєС‚СѓСЂР° "Р—Р°РїСЂРѕСЃ"
{
    char request[SIZE];
    struct job *next;
} job;

typedef struct queue_job // СЃС‚СЂСѓРєС‚СѓСЂР° "РћС‡РµСЂРµРґСЊ Р·Р°РїСЂРѕСЃРѕРІ"
{
    job *head;
    job *tail;
} queue_job;

queue_job q = {NULL, NULL}; 
sem_t semaphore;
FILE* resFile;
pthread_mutex_t mutex;
pthread_mutex_t mutex1;

void enqueue_job(queue_job *queue, job *new)
{
	pthread_mutex_lock(&mutex1);
    job *new_job = malloc(sizeof(job));
    strcpy(new_job->request, new->request);
    new_job->next = NULL;

    if (queue->head == NULL)
    {
        queue->head = new_job;
        queue->tail = new_job;
    }
    else
    {
        queue->tail->next = new_job;
        queue->tail = new_job;
    }
    sem_post(&semaphore);
    pthread_mutex_unlock(&mutex1);
}

job *dequeue_job(queue_job *queue)
{
	
    if (queue->head == NULL)
        return NULL;

    job *head = malloc(sizeof(job));
    strcpy(head->request, queue->head->request);
    head->next = queue->head->next;

    queue->head->next = NULL;
    free(queue->head);

    job *new_head = head->next;
    queue->head = new_head;

    return head;
}
void pregen()
{
	FILE* file = fopen("pregen.txt", "w");
	for (int i = 0; i < 100000; ++i)
	{
		fputs("Command в„–", file);
		char num[5];
		sprintf(num, "%d", i);
		fputs(num, file);
		fputs("\n", file);
	}
	fclose(file);
}
void readFile(FILE* file)
{
	char string[SIZE];
	while(fgets(string, SIZE, file) != NULL)
	{
		enqueue_job(&q, string);
	}	
}
void writeJob()
{	
	pthread_mutex_lock(&mutex);
	job* j = dequeue_job(&q);
	if (j != NULL)
		fputs(j->request, resFile);
	pthread_mutex_unlock(&mutex);
}
void threadsInfinity()
{
	while(q.head != NULL)
	{
		sem_wait(&semaphore);
		writeJob();
	}
}
int main(int argc, char const *argv[])
{	
	if (argc != 2)
	{
		printf("Bad input\n");
		return -1;
	}
	pregen();	
	sem_init(&semaphore, 0, 1);
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex1, NULL);



	resFile = fopen("result.txt", "w");
	if (resFile == NULL)
	{
		printf("File is not opened\n");
		return -1;
	}
	FILE* file = fopen("pregen.txt", "r");
	if (file == NULL)
	{
		printf("File is not opened\n");
		return -1;
	}
	
	

	//int N = atoi(argv[1]);
	int N = 2;
	if (N <= 0)
	{
		perror("input");
		return -1;
	}
	pthread_t* threads = malloc(N * sizeof(pthread_t));
	for (int i = 0; i < N; ++i)
	{		
		pthread_create(&threads[i], NULL, threadsInfinity, NULL);
	}
	readFile(file);	
	fclose(file);
	for (int i = 0; i < N; ++i)
	{		
		pthread_join(threads[i], NULL);
	}
	fclose(resFile);
	sem_destroy(&semaphore);
	pthread_mutex_destroy(&mutex);
	return 0;
}
