#include <stdio.h>

#include "task.h"



void task_1(void *args)
{
    int *ints = (int *)args;
    printf("%d, %d, %d\n", ints[0], ints[1], ints[2]);
}


int main()
{
    int args[] = {5, 6, 7};
    task_t *t = task_create(task_1, args);

    task_exec(t);

    task_destroy(t);

    return 0;
}


