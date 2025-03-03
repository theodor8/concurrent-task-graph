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

typedef struct
{
    size_t id;
    tthreads_t *tthreads;
} thread_args_t;


static void *task_thread(void *arg)
{
    thread_args_t *args = (thread_args_t *)arg;
    size_t id = args->id;
    tthreads_t *tthreads = args->tthreads;
    free(args);
    while (true)
    {
        printf("Thread %zu waiting for task\n", id);
        sem_wait(&tthreads->tasks_sem);
        printf("Thread %zu getting task\n", id);

        // TODO: mutex
        task_t *task = tasks_get(tthreads->tasks);
        printf("Thread %zu executing task\n", id);
        task_exec(task);
        /*task_destroy(task);*/
        // TODO: dependencies
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

    thread_args_t *args;
    for (size_t i = 0; i < num_threads; ++i)
    {
        args = calloc(1, sizeof(thread_args_t));
        args->id = i;
        args->tthreads = tthreads;
        if (pthread_create(&tthreads->threads[i], NULL, task_thread, args) != 0)
        {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
        printf("Created thread %zu\n", i);
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

void tthreads_purge(tthreads_t *tthreads)
{
    // TODO: free completed tasks
}



void tthreads_add(tthreads_t *tthreads, void (*func)(void *), void *args)
{
    // TODO: mutex
    tasks_add(tthreads->tasks, func, args);
    sem_post(&tthreads->tasks_sem);
}






