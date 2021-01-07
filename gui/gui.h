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

#ifndef GUI_H
#define GUI_H

enum
{
    OPEN_WINDOW_EVENT,
    CLOSE_WINDOW_EVENT,
    MOVE_WINDOW_EVENT,
    RESIZE_WINDOW_EVENT
};

typedef void * window_t;

struct window_attr
{
    char title[64];
    int x, y, width, height;
};

struct window_event
{
    window_t window;
    int type, params[16]; // max 16 params per event, can be changed
};

window_t open_window(char *title);
void close_window(window_t window);
void read_window(window_t window, struct window_attr *attr);
void write_window(window_t window, struct window_attr *attr);
int poll_event(struct window_event *event);

#endif
