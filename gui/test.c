/*
 * gui.h testing program
 */

#include "gui.h"
#include <stdio.h>

int main(void)
{
    window_t window = open_window("Test");

    int running = 1;

    struct window_event event;

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
                case KEY_DOWN_EVENT:
                {
                    if(event.params[0] == KEY_UP)
                    {
                        printf("/\\n");
                    }

                    break;
                }
            }
        }
    }

    close_window(window);

    return 0;
}
