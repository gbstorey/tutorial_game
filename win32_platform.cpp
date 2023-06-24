//
// Created by Garrett on 6/23/2023.
//
#include <windows.h>
#include <iostream>

#include "renderer.cpp"
#include "platform_common.cpp"
#include "game.cpp"

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

    Input input = {};

    float delta_time = 0.016666f;
    LARGE_INTEGER frame_begin_time;
    QueryPerformanceCounter(&frame_begin_time);
    float performance_frequency;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performance_frequency = (float)perf.QuadPart;
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
    while (running) {
        // Input
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0,
                           0, PM_REMOVE)) {
            switch(message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    auto vk_code = (u32)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);
#define process_button(b, vk)\
case vk: {\
input.buttons[b].is_down = is_down;\
input.buttons[b].changed = true;\
} break;
                    switch (vk_code) {
                        process_button(BUTTON_UP, VK_UP)
                        process_button(BUTTON_DOWN, VK_DOWN)
                        process_button(BUTTON_LEFT, VK_LEFT)
                        process_button(BUTTON_RIGHT, VK_RIGHT)
                        process_button(BUTTON_W, 0x57)
                        process_button(BUTTON_S, 0x53)
                        default: break;
                    }
                } break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }
        // Simulate
        simulate_game(&input, delta_time);

        // Render
        StretchDIBits(hdc, 0, 0, render_state.width,
                      render_state.height, 0, 0, render_state.width,
                      render_state.height, render_state.memory, &render_state
                      .bit_map_info, DIB_RGB_COLORS,
                      SRCCOPY);

        LARGE_INTEGER frame_end_time;
        QueryPerformanceCounter(&frame_end_time);
        delta_time = (float)
                (frame_end_time.QuadPart - frame_begin_time.QuadPart)/
                performance_frequency;
        if (delta_time > 0.1) delta_time = 0.1;
        frame_begin_time = frame_end_time;
    }
#pragma clang diagnostic pop
    return 0;
}
