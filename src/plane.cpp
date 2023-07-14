#include <cstdint>
#include <glad/glad.h>
#include <iostream>
#include "texture.h"
#include "model.h"
#include "plane.h"

#include "shader.h"
#include "RenderContext.h"

static void plane_load_vertex_data(uint32_t& vao, uint32_t& vbo, uint32_t& ebo);

static Vertex plane_data[] = {
    Vertex{glm::vec3(-0.5, 0.5, 0.0), glm::vec3(0.0), glm::vec2(0.0, 1.0)},
    Vertex{glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0.0), glm::vec2(0.0, 0.0)},
    Vertex{glm::vec3(0.5, -0.5, 0.0), glm::vec3(0.0), glm::vec2(1.0, 0.0)},
    Vertex{glm::vec3(0.5, 0.5, 0.0), glm::vec3(0.0), glm::vec2(1.0, 1.0)}
};

static uint32_t plane_vertex_indices[] = {
    0, 1, 2,
    2, 3, 0
};

static void plane_load_vertex_data(uint32_t& vao, uint32_t& vbo, uint32_t& ebo){
    // Initialize vertex data and attributes
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_data), plane_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv_coords));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_vertex_indices), plane_vertex_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Plane::Plane() :
    transform(glm::vec3(0.0), glm::vec3(3.0), Rotation(glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0))){
    plane_load_vertex_data(vao, vbo, ebo);
    shader_id = shader_id::BASIC;

    if(!load_texture_make_id("assets/wall.jpg", &texture.id)){
        std::cerr << "Error: Failed to load texture\n";
        ok = false;
    }
    ok = true;
}

void Plane::draw(const glm::mat4& view){
    Shader& shader = RobyRender::shaders[shader_id::BASIC];
    shader.use();
    shader.set_mat4("MVP", RobyRender::projection * view * transform.world());

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR){
        std::cerr << "GL Error at init: " << err << "\n";
    }
}
