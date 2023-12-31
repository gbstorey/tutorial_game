//
// Created by Garrett on 6/23/2023.
//

#include "main.h"
#include "util.cpp"

internal void clear_screen(u32 color) {
    auto* pixel = (u32*)render_state.memory;
    for (int y=0; y < render_state.height; y++) {
        for (int x=0; x < render_state.width; x++) {
            *pixel++ = color;
        }
    }
}



internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {
    x0 = clamp(0, x0, render_state.width);
    x1 = clamp(0, x1, render_state.width);
    y0 = clamp(0, y0, render_state.height);
    y1 = clamp(0, y1, render_state.height);


    for (int y=y0; y < y1; y++) {
        auto* pixel = (u32*)render_state.memory + x0 +
                y*render_state.width;
        for (int x=x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

global_variable float render_scale = 0.01f;

internal void draw_rect(float x, float y, float half_size_x, float
half_size_y, u32 color) {
    x *= (float)render_state.height * render_scale;
    y *= (float)render_state.height * render_scale;
    half_size_x *= (float)render_state.height * render_scale;
    half_size_y *= (float)render_state.height * render_scale;

    x += (float)render_state.width / 2.f;
    y += (float)render_state.height / 2.f;

    int x0 = (int)(x - half_size_x);
    int x1 = (int)(x + half_size_x);
    int y0 = (int)(y - half_size_y);
    int y1 = (int)(y + half_size_y);
    draw_rect_in_pixels(x0, y0, x1, y1, color);
}