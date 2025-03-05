#include <stdio.h>
#include <unistd.h>

#include "tthreads.h"



void task_print(void *arg)
{
    char *str = (char *)arg;
    printf("%s\n", str);
    free(str);
}

void task_sleep(void *arg)
{
    printf("task_sleep\n");
    sleep(1);
}



int main()
{
    tthreads_t *tthreads = tthreads_create(5);

    char *str = NULL;
    task_t *prev_task = NULL;
    for (int i = 0; i < 10; ++i)
    {
        str = calloc(20, sizeof(char));
        sprintf(str, "task %d", i);
        task_t **deps = calloc(1, sizeof(task_t *));
        deps[0] = prev_task;
        prev_task = tthreads_add(tthreads, task_print, str, deps, prev_task ? 1 : 0);
        free(deps);
    }

    /**/
    /*task_t *ts = tthreads_add(tthreads, task_sleep, NULL, NULL, 0);*/
    /*char *str = NULL;*/
    /*for (int i = 0; i < 5; ++i) {*/
    /*    str = calloc(20, sizeof(char));*/
    /*    sprintf(str, "task %d", i);*/
    /*    task_t **deps = calloc(1, sizeof(task_t *));*/
    /*    deps[0] = ts;*/
    /*    tthreads_add(tthreads, task_print, str, deps, 1);*/
    /*    free(deps);*/
    /*}*/


    sleep(2);
    

    tthreads_destroy(tthreads);

    return 0;
}


