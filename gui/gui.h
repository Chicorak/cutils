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

enum event_type
{
    CLOSE_WINDOW_EVENT,
    KEY_DOWN_EVENT,
    KEY_UP_EVENT,
    MOUSE_CLICK_EVENT,
    MOUSE_MOVE_EVENT
};

enum special_key_code
{
    KEY_UP = 256,
    KEY_DOWN = 257,
    KEY_LEFT = 258,
    KEY_RIGHT = 259
};

enum mouse_buttons
{
    LEFT_MOUSE_BUTTON,
    RIGHT_MOUSE_BUTTON,
    MIDDLE_MOUSE_BUTTON
};

typedef void * window_t;
typedef void * graphics_t;

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

struct color
{
    unsigned char r, g, b;
};

window_t open_window(char *title);
int close_window(window_t window);
int read_window(window_t window, struct window_attr *attr);
int write_window(window_t window, struct window_attr *attr);
int poll_event(struct window_event *event);
graphics_t create_graphics(window_t window);
int destroy_graphics(window_t window, graphics_t graphics);
int clear_window(window_t window, graphics_t graphics);
int draw_pixel(window_t window, graphics_t graphics, int x, int y, struct color *color);
int draw_rect(window_t window, graphics_t graphics, int x, int y, int width, int height, struct color *color);
int fill_rect(window_t window, graphics_t graphics, int x, int y, int width, int height, struct color *color);

#endif
