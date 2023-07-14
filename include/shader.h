#pragma once
#include <string>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum shader_id{
    BASIC,
    NO_TEXTURE,

    SHADERS_NUM
};

struct Shader{
    void use();
    void set_mat4(const char *name, const glm::mat4& m);
    void set_int(const char *name, int value);
    void set_bool(const char *name, bool value);
    void set_float(const char *name, float value);
    uint32_t program_id;
};

bool load_shader(Shader * shader, const char *vertex_shader_path, const char *frag_shader_path);
