//
// Created by Garrett on 6/24/2023.
//

struct Button_State {
    bool is_down;
    bool changed;
};

enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_W,
    BUTTON_S,
    BUTTON_COUNT
};

struct Input {
    Button_State buttons[BUTTON_COUNT];
};
