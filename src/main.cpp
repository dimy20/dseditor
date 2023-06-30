#include <glm/fwd.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include "mesh.h"

#define SCREEN_W 800
#define SCREEN_H 600
GLFWwindow * window;

void do_input(){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
};

void log_opengl_info(){
    printf("%s\n",glGetString(GL_VENDOR));
    printf("%s\n",glGetString(GL_RENDERER));
    printf("%s\n",glGetString(GL_VERSION));
};

void fb_resize_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void GLAPIENTRY gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                              GLsizei length, const GLchar* message, const void* userParam){
    // Print or handle the debug message as needed
    // Example: Print the message to the console
    std::cout << "OpenGL Debug Message: " << message << std::endl;
}

void init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    PANIC(!(window = glfwCreateWindow(SCREEN_W, SCREEN_H, "opengl", NULL, NULL)), 
            "Failed to create window");

    glfwMakeContextCurrent(window);
    PANIC(!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)), "Failed to initialize glad");

    glfwSetFramebufferSizeCallback(window, fb_resize_callback);

    glViewport(0, 0, SCREEN_W, SCREEN_H);
    log_opengl_info();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}

void render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

int main(){
    init();
    Model teapot;
    if(!load_model(&teapot, "teapot.obj")){
        std::cerr << "Failed to load model";
        return 1;
    }

    auto shader = load_shader("shaders/vertex.glsl", "shaders/frag.glsl");

    if(!shader.has_value()){
        std::cerr << "Failed to load shader\n";
    }

    Texture texture;
    if(!load_texture_make_id("assets/wall.jpg", &texture.id)){
        std::cerr << "Error: Failed to load texture\n";
    }


    while(!glfwWindowShouldClose(window)){
        do_input();
        render();

        glm::mat4 view(1.0), model(1.0), projection(1.0);
        //model = glm::scale(model, glm::vec3(10.0));
        model = glm::rotate(model, glm::radians(10 * (float)glfwGetTime()), glm::vec3(1.0, 0.0f, 0.0));
        view = glm::translate(view, glm::vec3(0.0, 0.0, -20));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        shader->use();
        shader->set_mat4("model", model);
        shader->set_mat4("projection", projection);
        shader->set_mat4("view", view);

        model_draw(&teapot, shader.value());

        glfwSwapBuffers(window);
        glfwPollEvents();


    };

    glfwTerminate();
    return 0;
}
