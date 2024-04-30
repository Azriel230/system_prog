#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct job //структура "Запрос"
{
    char* request;
    job* next;
} job;

typedef struct queue_job
{
    job* head;
    job* tail;
} queue_job;

void enqueue_job(queue_job* queue, job* new)
{
    job* new_job = malloc(sizeof(queue_job));
    strcpy(new_job->request, new->request);
    new_job->next = NULL;
    queue->tail->next = new_job;
}

job* dequeue_job(queue_job* queue)
{
    job* head = malloc(sizeof(queue_job));
    strcpy(head->request, queue->head->request);
    head->next = queue->head->next;

    queue->head->request = '0';
    queue->head->next = NULL;
    free(queue->head);

    job* new_head = head->next;
    queue->head = new_head;

    return head;
}

int main()
{



    return 0;
}