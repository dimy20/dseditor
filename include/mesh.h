#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include "texture.h"

struct Vertex{
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
    std::vector<Mesh> meshes;
    std::string directory;
};

bool load_model(Model* model, const char* filename);
void model_draw(Model *model, const Shader& shader);
