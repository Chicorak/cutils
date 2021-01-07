/* gui.h - a simple, lightweight, easy to use, cross platform gui library
 *
 * Copyright (c) 2021 Cleanware
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

#include "gui.h"
#include <stdlib.h>
#include <X11/Xlib.h>

struct window
{
    Window window;
    Display *display;
};

window_t open_window(char *title)
{
    Display *display = XOpenDisplay(NULL);

    if(display == NULL)
        return NULL;

    int screen = DefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        0,
        0,
        640,
        480,
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen)
    );

    XStoreName(display, window, title);

    XSelectInput(display, window, ExposureMask | KeyPressMask | KeyReleaseMask);
    XMapWindow(display, window);

    struct window *wnd = (struct window *)malloc(sizeof(struct window));

    wnd->window = window;
    wnd->display = display;

    return (window_t)wnd;
}

void close_window(window_t window)
{
    struct window *wnd = (struct window *)window;

    XDestroyWindow(wnd->display, wnd->window);
    free(window);
}

void read_window(window_t window, struct window_attr *attr)
{
    struct window *wnd = (struct window *)window;

    XWindowAttributes xattr;
    XGetWindowAttributes(wnd->display, wnd->window, &xattr);

    char *name = "\0";

    XFetchName(wnd->display, wnd->window, &name);

    int i;
    for(i = 0; i < 64; i++)
    {
        if(name[i] == '\0')
        {
            attr->title[i] = '\0';
            break;
        }

        attr->title[i] = name[i];
    }

    attr->x = xattr.x;
    attr->y = xattr.y;
    attr->width = xattr.width;
    attr->height = xattr.height;
}

void write_window(window_t window, struct window_attr *attr)
{
    struct window *wnd = (struct window *)window;

    XStoreName(wnd->display, wnd->window, attr->title);

    XWindowChanges changes;

    changes.x = attr->x;
    changes.y = attr->y;
    changes.width = attr->width;
    changes.height = attr->height;

    XConfigureWindow(wnd->display, wnd->window, CWX | CWY | CWWidth | CWHeight, &changes);
}
