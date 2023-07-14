#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "texture.h"

#include "transform.h"

struct Vertex{
    Vertex() {};
    Vertex(const glm::vec3& _position, const glm::vec3& _normal, const glm::vec2& _uv_coords);
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv_coords;
};

struct Mesh{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;
    uint32_t vao, vbo, ebo;
};

void init_mesh(Mesh *mesh);

struct Model{
    Model(const Transform& _transform) : transform(_transform) {};
    void draw(const glm::mat4& view);

    std::vector<Mesh> meshes;
    std::string directory;
    Transform transform;
    uint32_t shader_id;
};

bool load_model(Model* model, const char* filename);
