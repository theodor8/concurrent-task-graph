#pragma once


typedef void (*task_func_t)(void *);
typedef struct task task_t;


task_t *task_create(task_func_t func, void *args);
void task_destroy(task_t *t);

void task_exec(task_t *t);


