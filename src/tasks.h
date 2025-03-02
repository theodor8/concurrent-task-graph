#pragma once


typedef struct task task_t;
typedef struct tasks tasks_t;

tasks_t *tasks_create();
void tasks_destroy(tasks_t *tasks);
void tasks_add(tasks_t *tasks, void (*func)(void *), void *args);
task_t *tasks_get(tasks_t *tasks);



void task_exec(task_t *task);
void task_destroy(task_t *task);


