#include "task.h"

#include <stdlib.h>



struct task
{
    task_func_t func;
    void *args;
};


task_t *task_create(task_func_t func, void *args)
{
    task_t *t = malloc(sizeof(task_t));
    t->func = func;
    t->args = args;
    return t;
}

void task_destroy(task_t *t)
{
    free(t);
}

void task_exec(task_t *t)
{
    t->func(t->args);
}

