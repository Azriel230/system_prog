#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct job // структура "Запрос"
{
    char request[1024];
    struct job *next;
} job;

typedef struct queue_job // структура "Очередь запросов"
{
    job *head;
    job *tail;
} queue_job;

void enqueue_job(queue_job *queue, job *new)
{
    job *new_job = malloc(sizeof(queue_job));
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
}

job *dequeue_job(queue_job *queue)
{
    if (queue->head == NULL)
        return NULL;

    job *head = malloc(sizeof(queue_job));
    strcpy(head->request, queue->head->request);
    head->next = queue->head->next;

    queue->head->next = NULL;
    free(queue->head);

    job *new_head = head->next;
    queue->head = new_head;

    return head;
}

int main()
{
    queue_job q = {NULL, NULL};
    job j4 = {"Command #4", NULL};
    job j3 = {"Command #3", &j4};
    job j2 = {"Command #2", &j3};
    job j1 = {"Command #1", &j2};
    enqueue_job(&q, &j1);
    enqueue_job(&q, &j2);
    enqueue_job(&q, &j3);
    enqueue_job(&q, &j4);

    while (q.head != NULL)
    {
        job* j = dequeue_job(&q);
        printf("%s\n" ,j->request);
    }

    return 0;
}
