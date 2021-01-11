/* gui_win32.c - gui.h implementation for win32
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

    ShowWindow(hwnd, SW_SHOW);

    return (window_t)hwnd;
}

void close_window(window_t window)
{
    DestroyWindow((HWND)window);
    free(window);
}

void read_window(window_t window, struct window_attr *attr)
{
    GetWindowText((HWND)window, attr->title, 64);

    RECT window_rect;

    if(GetWindowRect((HWND)window, &window_rect))
    {
        attr->x = window_rect.left;
        attr->y = window_rect.right;
        attr->width = window_rect.right - window_rect.left;
        attr->height = window_rect.bottom - window_rect.top;
    }
    else
    {
        attr->x = -1;
        attr->y = -1;
        attr->width = -1;
        attr->height = -1;
    }
}

void write_window(window_t window, struct window_attr *attr)
{
    SetWindowText((HWND)window, attr->title);

    SetWindowPos(
        (HWND)window,
        HWND_TOP,
        attr->x,
        attr->y,
        attr->width,
        attr->height,
        0
    );
}

int poll_event(struct window_event *event)
{
    MSG msg;

    if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
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

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        return 1;
    }
    else return 0;
}
