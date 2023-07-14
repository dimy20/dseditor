#pragma once

#include <SDL2/SDL.h>
#define MAX_KEYBOARD_KEYS 350

namespace RobyInput{
    enum Key{
        A = SDL_SCANCODE_A,
        W = SDL_SCANCODE_W,
        S = SDL_SCANCODE_S,
        D = SDL_SCANCODE_D,
    };

    void init();
    bool is_key_pressed(int key);
    void process();
    extern int mouse_x;
    extern int mouse_y;
    extern bool is_running;
}

