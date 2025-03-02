#include "tthreads.h"

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

#include "tasks.h"





struct tthreads
{
    pthread_t *threads;
    size_t num_threads;
    tasks_t *tasks;
    sem_t tasks_sem;
};


static void *task_thread(void *arg)
{
    tthreads_t *tthreads = (tthreads_t *)arg;
    while (true)
    {
        sem_wait(&tthreads->tasks_sem);
        task_t *task = tasks_get(tthreads->tasks);
        task_exec(task);
        task_destroy(task);
    }
    return NULL;
}


tthreads_t *tthreads_create(size_t num_threads)
{
    tthreads_t *tthreads = calloc(1, sizeof(tthreads_t));
    tthreads->threads = calloc(num_threads, sizeof(pthread_t));
    tthreads->num_threads = num_threads;
    tthreads->tasks = tasks_create();
    sem_init(&tthreads->tasks_sem, 0, 0);

    for (size_t i = 0; i < num_threads; ++i)
    {
        if (pthread_create(&tthreads->threads[i], NULL, task_thread, tthreads) != 0)
        {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
        printf("Created thread\n");
    }

    return tthreads;
}

void tthreads_destroy(tthreads_t *tthreads)
{
    for (int i = 0; i < tthreads->num_threads; ++i)
    {
        pthread_cancel(tthreads->threads[i]);
        pthread_join(tthreads->threads[i], NULL);
    }
    free(tthreads->threads);
    tasks_destroy(tthreads->tasks);
    sem_destroy(&tthreads->tasks_sem);
    free(tthreads);
}



void tthreads_add(tthreads_t *tthreads, void (*func)(void *), void *args)
{
    tasks_add(tthreads->tasks, func, args);
    sem_post(&tthreads->tasks_sem);
}






