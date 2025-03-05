#pragma once

#include <stdlib.h>


typedef struct task task_t;
typedef struct tthreads tthreads_t; // task threads

tthreads_t *tthreads_create(size_t num_threads);
void tthreads_destroy(tthreads_t *tthreads);

void tthreads_purge(tthreads_t *tthreads);

task_t *tthreads_add(tthreads_t *tthreads, void (*func)(void *), void *arg, task_t **deps, size_t ndeps);


