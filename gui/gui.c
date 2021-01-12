/* gui.c - gui.h platform implementation
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

#if defined(__linux__)
    #if !defined(WAYLAND) && !defined(X11)
        #define X11
    #endif
#endif

#include "gui.h"

/*----------------------------------------------------------------------------*/
/*                           Windows Implementation                           */
/*----------------------------------------------------------------------------*/

#if defined(_WIN32)

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    switch(umsg)
    {
        case WM_QUIT:
        case WM_CLOSE:
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, umsg, wparam, lparam);
}

window_t open_window(char *title)
{
    WNDCLASSEX wc = { };
    HINSTANCE hInstance = GetModuleHandle(NULL);

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = title;

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        title,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if(hwnd == NULL) return NULL;

    ShowWindow(hwnd, SW_SHOW);

    return (window_t)hwnd;
}

int close_window(window_t window)
{
    return DestroyWindow((HWND)window);
}

int read_window(window_t window, struct window_attr *attr)
{
    int result = 0;

    if(!GetWindowText((HWND)window, attr->title, 64)) result++;

    RECT rect;

    if(GetWindowRect((HWND)window, &rect))
    {
        attr->x = rect.left;
        attr->y = rect.right;
        attr->width = rect.right - rect.left;
        attr->height = rect.bottom - rect.top;
    }
    else
    {
        attr->x = -1;
        attr->y = -1;
        attr->width = -1;
        attr->height = -1;

        result += 2;
    }

    return result;
}

/* TODO: Figure out why MoveWindow doesnt work */
int write_window(window_t window, struct window_attr *attr)
{
    return 1; /* temporarily return failure */

    /*int result = 0;

    if(!SetWindowText((HWND)window, attr->title)) result++;
    if(!MoveWindow((HWND)window, attr->x, attr->y, attr->width, attr->height, 0)) result += 2;

    return result;*/
}

int poll_event(struct window_event *event)
{
    MSG msg;

    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        event->window = (window_t)msg.hwnd;

        switch(msg.message)
        {
            case WM_QUIT:
            case WM_CLOSE:
            case WM_DESTROY:
            {
                event->type = CLOSE_WINDOW_EVENT;

                break;
            }
            case WM_KEYDOWN:
            {
                event->type = KEY_DOWN_EVENT;

                event->params[0] = (int)msg.wParam;

                break;
            }
            case WM_CHAR:
            {

                break;
            }
            case WM_KEYUP:
            {
                event->type = KEY_UP_EVENT;

                event->params[0] = (int)msg.wParam;

                break;
            }
            case WM_MOUSEMOVE:
            {
                event->type = MOUSE_MOVE_EVENT;

                event->params[0] = (int)(short) LOWORD(msg.lParam);
                event->params[1] = (int)(short) HIWORD(msg.lParam);

                break;
            }
            case WM_LBUTTONDOWN:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = LEFT_MOUSE_BUTTON;

                break;
            }
            case WM_LBUTTONUP:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = LEFT_MOUSE_BUTTON;

                break;
            }
            case WM_LBUTTONDBLCLK:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = LEFT_MOUSE_BUTTON;

                break;
            }
            case WM_RBUTTONDOWN:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = RIGHT_MOUSE_BUTTON;

                break;
            }
            case WM_RBUTTONUP:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = RIGHT_MOUSE_BUTTON;

                break;
            }
            case WM_RBUTTONDBLCLK:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = RIGHT_MOUSE_BUTTON;

                break;
            }
            case WM_MBUTTONDOWN:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = MIDDLE_MOUSE_BUTTON;

                break;
            }
            case WM_MBUTTONUP:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = MIDDLE_MOUSE_BUTTON;

                break;
            }
            case WM_MBUTTONDBLCLK:
            {
                event->type = MOUSE_CLICK_EVENT;

                event->params[0] = MIDDLE_MOUSE_BUTTON;

                break;
            }
        }

        return 1;
    }
    else return 0;
}

graphics_t create_graphics(window_t window)
{
    return (graphics_t)GetDC((HWND)window);
}

int destroy_graphics(window_t window, graphics_t graphics)
{
    return DeleteDC((HDC)graphics);
}

int clear_window(window_t window, graphics_t graphics)
{
    RECT rect;

    if(GetWindowRect((HWND)window, &rect))
    {
        return RedrawWindow((HWND)window, &rect, NULL, RDW_ERASE);
    }
    else return 0;
}

int draw_pixel(window_t window, graphics_t graphics, int x, int y, struct color *color)
{
    return SetPixel((HDC)graphics, x, y, RGB(color->r, color->g, color->b));
}

#endif

/*----------------------------------------------------------------------------*/
/*                             UWP Implementation                             */
/*----------------------------------------------------------------------------*/

#if defined(_WIN32) && defined(UWP)



#endif

/*----------------------------------------------------------------------------*/
/*                             X11 Implementation                             */
/*----------------------------------------------------------------------------*/

#if defined(__linux__) && defined(X11)

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

#endif

/*----------------------------------------------------------------------------*/
/*                           Wayland Implementation                           */
/*----------------------------------------------------------------------------*/

#if defined(__linux__) && defined(WAYLAND)



#endif

/*----------------------------------------------------------------------------*/
/*                            Cocoa Implementation                            */
/*----------------------------------------------------------------------------*/

#if defined(__APPLE__)



#endif

/*----------------------------------------------------------------------------*/
/*                           Android Implementation                           */
/*----------------------------------------------------------------------------*/

#if defined(__ANDROID__)



#endif
