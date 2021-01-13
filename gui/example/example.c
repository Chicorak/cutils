/*
 * gui.h testing program
 */

#include "gui.h"
#include <stdio.h>

int main(void)
{
    window_t window = open_window("Test");

    if(window != NULL)
    {
        struct window_attr attr;

        read_window(window, &attr);

        graphics_t g = create_graphics(window);

        struct color c = { 255, 0, 0 };

        int running = 1;

        struct window_event event;

        int x, y;
        for(x = 0; x < attr.width; x++)
        {
            for(y = 0; y < attr.height; y++)
            {
                draw_pixel(window, g, x, y, &c);
            }
        }

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

        destroy_graphics(window, g);
        close_window(window);
    }
    else
    {
        printf("error: failed to open window\n");
        return 1;
    }

    return 0;
}
