#include <stdio.h>
#include <unistd.h>

#include "tthreads.h"



void task_print(void *arg)
{
    printf("%s\n", (char *)arg);
}

void task_add_tasks(void *arg)
{
    tthreads_t *tthreads = (tthreads_t *)arg;
    char str[50];
    for (int i = 0; i < 10; ++i)
    {
        sprintf(str, "Task %d", i);
        tthreads_add(tthreads, task_print, str);
        sleep(1);
    }
}




int main()
{
    tthreads_t *tthreads = tthreads_create(3);


    for (int i = 0; i < 5; ++i)
    {
        tthreads_add(tthreads, task_print, "task");
    }
    
    sleep(1);

    tthreads_destroy(tthreads);

    return 0;
}


