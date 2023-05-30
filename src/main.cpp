#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils.h"
#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_W 800
#define SCREEN_H 600
GLFWwindow * window;
bool running = true;

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
}

void render(){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

int main(){
	init();

  auto shader = load_shader("shaders/vertex.glsl", "shaders/frag.glsl");

  if(!shader.has_value()){
      std::cerr << "Failed to load shader\n";
  }

  auto _texture = load_texture("assets/wall.jpg");

  Texture texture;
  if(!_texture.has_value()) std::cerr << "Failed to Load texture";
  texture = _texture.value();

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	uint32_t indices [] ={
		0, 1, 3,
		1, 2, 3
	};

	uint32_t vbo, vao, ebo;
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	// drawing order
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR){                                                   
		fprintf(stderr, "Error: %d\n", err);
		exit(1);                                                                                  
	}        

	while(!glfwWindowShouldClose(window)){
      do_input();

      render();

      glm::mat4 view(1.0), model(1.0), projection(1.0);
      model = glm::rotate(model, glm::radians(10 * (float)glfwGetTime()), glm::vec3(1.0, 0.0f, 0.0));
      view = glm::translate(view, glm::vec3(0.0, 0.0, -2));
      projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

      shader->use();
      shader->set_mat4("model", model);
      shader->set_mat4("projection", projection);
      shader->set_mat4("view", view);

      glBindTexture(GL_TEXTURE_2D, texture.id);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
	};

	glfwTerminate();
	return 0;
}
