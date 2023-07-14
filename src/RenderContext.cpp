#include "RenderContext.h"
#include <cstdint>
#include <glm/glm.hpp>
#include "utils.h"
#include <iostream>
#include "input.h"

#include <SDL2/SDL.h>

static SDL_Window *window;
static SDL_GLContext gl_context;
static uint32_t prev_time;

namespace RobyRender{
    float delta_time = 0.0f;
    Shader shaders[SHADERS_NUM];
    glm::mat4 projection;
};

void log_opengl_info(){
    printf("%s\n",glGetString(GL_VENDOR));
    printf("%s\n",glGetString(GL_RENDERER));
    printf("%s\n",glGetString(GL_VERSION));
};

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam){
    std::cout << "OpenGL debug message: " << message << std::endl;
}

static void update_time(){
    uint32_t curr_time = SDL_GetTicks();
    RobyRender::delta_time = static_cast<float>((curr_time - prev_time) / 1000.0f);
    prev_time = curr_time;
}

bool init_sdl_and_opengl(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return false;
    }

    // context attribs
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    window = SDL_CreateWindow("Roby",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_W,
                              SCREEN_H,
                              flags);

    if(!window){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);

    if(!gl_context){
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        return false;
    }

    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        std::cerr << "Error: Failed to initialize glad pointers!\n";
        return false;
    };

    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    //glEnable(GL_PROGRAM_POINT_SIZE);
    glDebugMessageCallback(gl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cerr << "GL Error at init: " << err << "\n";
        return false;
    }

    //SDL_ShowCursor(SDL_DISABLE);

    return true;
};

void RobyRender::quit(){
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
};

bool RobyRender::init(){
    if(!init_sdl_and_opengl()) return false;

    if(!load_shader(&shaders[shader_id::BASIC], 
                "shaders/vertex.glsl", "shaders/frag.glsl")){
        return false;
    }

    if(!load_shader(&shaders[shader_id::NO_TEXTURE],
                    "shaders/vertex/no_texture.glsl",
                    "shaders/fragment/no_texture.glsl")){
        return false;
    }

    RobyInput::init();
    projection = glm::perspective(glm::radians(45.0f), ASPECT_RATIO, 0.1f, 100.f);
    prev_time = static_cast<float>(SDL_GetTicks()) / 1000.0f;
    return true;
};

void RobyRender::clear(){
    update_time();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

};

void RobyRender::swap_buffers(){ SDL_GL_SwapWindow(window); }
