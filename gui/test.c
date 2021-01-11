/*
 * gui.h testing program
 */

#include "gui.h"
#include <stdio.h>

int main(void)
{
    int result = 0;
    window_t window = open_window("Test");

    if(window != NULL)
    {
        struct window_attr attr;

        if(read_window(window, &attr) == 0)
        {
            attr.width = 640;
            attr.height = 480;

            if(write_window(window, &attr) == 0)
            {
                graphics_t g = create_graphics(window);

                struct color c = { 255, 0, 0 };

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
                        }
                    }

                    fill_rect(window, g, 50, 50, 50, 50, &c);
                }

                if(!destroy_graphics(window, g))
                {
                    printf("error: failed to destroy graphics\n");
                    result = 4;
                }

                if(!close_window(window))
                {
                    printf("error: failed to close window\n");
                    result = 5;
                }
            }
            else
            {
                printf("error: failed to write to window\n");
                result = 3;
            }
        }
        else
        {
            printf("error: failed to read window\n");
            result = 2;
        }
    }
    else
    {
        printf("error: failed to open window\n");
        result = 1;
    }

    return result;
}
