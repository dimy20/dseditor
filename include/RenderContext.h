#pragma once
#include "shader.h"

#define SCREEN_W 800
#define SCREEN_H 600
#define ASPECT_RATIO SCREEN_W / static_cast<float>(SCREEN_H)

namespace RobyRender{
    bool init();
    void process_input();
    void clear();
    void quit();
    void swap_buffers();
    extern float delta_time;
    extern Shader shaders[SHADERS_NUM];
    extern glm::mat4 projection;
};
