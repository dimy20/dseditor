#include <SDL2/SDL_events.h>
#include <cstring>
#include "Camera.h"
#include "input.h"
#include <SDL2/SDL.h>

static bool keyboard[MAX_KEYBOARD_KEYS];

namespace RobyInput{
    int mouse_x;
    int mouse_y;
    bool is_running = true;
}

void RobyInput::init(){
    memset(keyboard, 0, sizeof(keyboard));
    is_running = true;
}

bool RobyInput::is_key_pressed(int key) { return keyboard[key]; }

static void do_keyup(const SDL_KeyboardEvent& e){
    if (e.repeat == 0 && e.keysym.scancode < MAX_KEYBOARD_KEYS){
        keyboard[e.keysym.scancode] = false;
    }
}

static void do_keydown(const SDL_KeyboardEvent& e){
    if (e.repeat == 0 && e.keysym.scancode < MAX_KEYBOARD_KEYS){
        keyboard[e.keysym.scancode] = true;
    }
}

void RobyInput::process(){
    SDL_Event e;
    SDL_GetGlobalMouseState(&RobyInput::mouse_x, &RobyInput::mouse_y);

    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT: is_running = false; break;
            case SDL_KEYDOWN: do_keydown(e.key); break;
            case SDL_KEYUP: do_keyup(e.key); break;
            //case SDL_MOUSEMOTION: handle_mouse_motion(); break;
            default: break;
        };
    }
}
