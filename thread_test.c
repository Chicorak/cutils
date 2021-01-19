#include "thread/thread.h"
#include <stdio.h>

int counter = 0;
mutex_t mutex;

void *trythis(void *arg)
{
    lock_mutex(mutex);

    counter++;

    printf("\n Job %d has started\n", counter);

    unsigned long i;
    for(i = 0; i < (0xFFFFFFFF); i++);

    printf("\n Job %d has finished\n", counter);

    unlock_mutex(mutex);

    return NULL;
}

int main(void)
{
    thread_t threads[2];

    mutex = create_mutex();

    int i;
    for(i = 0; i < 2; i++)
    {
        threads[i] = create_thread(trythis, NULL);

        if(threads[i] == NULL)
        {
            printf("failed to create thread %d, aborting\n", i);
            return -1;
        }
    }

    join_thread(threads[0]);
    join_thread(threads[1]);
    destroy_mutex(mutex);

    return 0;
}
