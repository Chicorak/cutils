/*
 * gui.h testing program
 */

#include "gui.h"
#include <stdio.h>

int main(void)
{
    window_t window = open_window("Test");

    if(window == NULL)
    {
        printf("error: failed to open window\n");
        return 1;
    }

    struct window_attr attr;

    read_window(window, &attr);

    attr.width = 640;
    attr.height = 480;

    write_window(window, &attr);

    struct window_event event;

    int running = 1;
    while(running)
    {
        while(poll_event(&event))
        {
            switch(event.type)
            {
                case CLOSE_WINDOW_EVENT:
                {
                    running = 0;
                    break;
                }
            }
        }
    }

    close_window(window);

    return 0;
}
