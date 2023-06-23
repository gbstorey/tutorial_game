//
// Created by Garrett on 6/23/2023.
//

#include <windows.h>

#ifndef TUTORIAL_GAME_MAIN_H
#define TUTORIAL_GAME_MAIN_H
#define global_variable static

#endif //TUTORIAL_GAME_MAIN_H

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bit_map_info;
};

global_variable Render_State render_state;