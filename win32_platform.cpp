//
// Created by Garrett on 6/23/2023.
//
#include <windows.h>
#include <iostream>

#include "renderer.cpp"

global_variable bool running = true;

LRESULT CALLBACK
window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_DESTROY: {
            running = false;
        }
            break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            int buffer_size =
                    render_state.width * render_state.height * (int)sizeof
                    (unsigned
                    int);

            // Release memory already allocated
            if (render_state.memory) VirtualFree(render_state.memory, 0,
                                           MEM_RELEASE);

            // Allocate memory from heap based on buffer size
            render_state.memory = VirtualAlloc(nullptr, buffer_size,
                                         MEM_COMMIT | MEM_RESERVE,
                                         PAGE_READWRITE);

            render_state.bit_map_info.bmiHeader.biSize =
                    sizeof(render_state.bit_map_info.bmiHeader);
            render_state.bit_map_info.bmiHeader.biWidth = render_state.width;
            render_state.bit_map_info.bmiHeader.biHeight = render_state.height;
            render_state.bit_map_info.bmiHeader.biPlanes = 1;
            render_state.bit_map_info.bmiHeader.biBitCount = 32;
            render_state.bit_map_info.bmiHeader.biCompression = BI_RGB;
        }
            break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
            int nShowCmd) {
    // Create Window Class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = window_callback;
    // Register Class
    RegisterClass(&window_class);
    // Create Window
    HWND window = CreateWindow(window_class.lpszClassName, "MyFirstGame!",
                               WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr,
                               nullptr, hInstance, nullptr);
    HDC hdc = GetDC(window);
#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
    while (running) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0,
                           0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // Simulate
        draw_rect(0, 0, 1, 1,
                  0x00ff00);
        draw_rect(30, 30, 5, 5,
                  0x00ff00);
        draw_rect(-20, 20, 8, 3,
                  0x00ff00);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width,
                      render_state.height, 0, 0, render_state.width,
                      render_state.height, render_state.memory, &render_state
                      .bit_map_info, DIB_RGB_COLORS,
                      SRCCOPY);
    }
#pragma clang diagnostic pop
    return 0;
}
