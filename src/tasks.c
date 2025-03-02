#include "tasks.h"

#include <stdlib.h>
#include <assert.h>



struct task
{
    void (*func)(void *);
    void *arg;
    task_t *next; // next in task list
};

struct tasks
{
    task_t *first;
    task_t *last;
};

tasks_t *tasks_create()
{
    tasks_t *tasks = calloc(1, sizeof(tasks_t));
    return tasks;
}

void tasks_destroy(tasks_t *tasks)
{
    task_t *task = tasks->first;
    free(tasks);
    while (task != NULL)
    {
        task_t *next = task->next;
        free(task);
        task = next;
    }
}

void tasks_add(tasks_t *tasks, void (*func)(void *), void *args)
{
    task_t *task = calloc(1, sizeof(task_t));
    task->func = func;
    task->arg = args;
    if (tasks->first == NULL)
    {
        assert(tasks->last == NULL);
        tasks->first = tasks->last = task;
    }
    else
    {
        tasks->last->next = task;
        tasks->last = task;
    }
}

task_t *tasks_get(tasks_t *tasks)
{
    assert(tasks->first != NULL && tasks->last != NULL);
    task_t *task = tasks->first;
    tasks->first = tasks->first->next;
    if (tasks->first == NULL)
    {
        assert(tasks->last == task);
        tasks->last = NULL;
    }
    return task;
}






void task_exec(task_t *task)
{
    task->func(task->arg);
}

void task_destroy(task_t *task)
{
    free(task);
}


