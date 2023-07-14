#pragma once

#include <cstdint>
#include "texture.h"
#include "transform.h"

struct RenderContext;

struct Plane{
    Plane();
    void draw(const glm::mat4& view);
    constexpr bool is_loaded() const { return ok; }

    Texture texture;
    Transform transform;
    uint32_t shader_id;

    private:
        uint32_t vao, vbo, ebo;
        bool ok;
};
