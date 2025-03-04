#include "tthreads.h"

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>



typedef struct task task_t;
struct task
{
    void (*func)(void *);
    void *arg;
    task_t *next;
};

static task_t *task_create(void (*func)(void *), void *arg)
{
    task_t *task = calloc(1, sizeof(task_t));
    task->func = func;
    task->arg = arg;
    task->next = NULL;
    return task;
}

static void task_exec(task_t *task)
{
    task->func(task->arg);
}



struct tthreads
{
    pthread_t *threads;
    size_t num_threads;
    task_t *task_first;
    task_t *task_curr;
    task_t *task_last;
    sem_t task_sem;
};

static task_t *get_task(tthreads_t *tthreads)
{
    assert(tthreads->task_first != NULL && tthreads->task_curr != NULL && tthreads->task_last != NULL);
    task_t *task = tthreads->task_curr;
    tthreads->task_curr = tthreads->task_curr->next;
    return task;
}

static void add_task(tthreads_t *tthreads, void (*func)(void *), void *arg)
{
    task_t *task = task_create(func, arg);
    if (tthreads->task_first == NULL)
    {
        assert(tthreads->task_curr == NULL && tthreads->task_last == NULL);
        tthreads->task_first = tthreads->task_curr = tthreads->task_last = task;
    }
    else
    {
        tthreads->task_last->next = task;
        tthreads->task_last = task;
        if (tthreads->task_curr == NULL)
        {
            tthreads->task_curr = task;
        }
    }
}



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
        sem_wait(&tthreads->task_sem);
        printf("Thread %zu getting task\n", id);

        // TODO: mutex
        task_t *task = get_task(tthreads);
        printf("Thread %zu executing task\n", id);
        task_exec(task);
        // TODO: dependencies
    }
    return NULL;
}


tthreads_t *tthreads_create(size_t num_threads)
{
    tthreads_t *tthreads = calloc(1, sizeof(tthreads_t));
    tthreads->threads = calloc(num_threads, sizeof(pthread_t));
    tthreads->num_threads = num_threads;
    sem_init(&tthreads->task_sem, 0, 0);

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
    task_t *task = tthreads->task_first;
    while (task) 
    {
        task_t *next = task->next;
        free(task);
        task = next;
    }
    sem_destroy(&tthreads->task_sem);
    free(tthreads);
}

void tthreads_purge(tthreads_t *tthreads)
{
    task_t *task = tthreads->task_first;
    while (task != tthreads->task_curr) 
    {
        task_t *next = task->next;
        free(task);
        task = next;
    }
}



void tthreads_add(tthreads_t *tthreads, void (*func)(void *), void *arg)
{
    // TODO: mutex
    add_task(tthreads, func, arg);
    sem_post(&tthreads->task_sem);
}






