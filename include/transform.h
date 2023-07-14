#pragma once

#include <glm/glm.hpp>
#include <ostream>

struct Rotation{
    Rotation() : angle(0.0), axis(glm::vec3(0.0)) {}
    Rotation(float _angle, const glm::vec3& _axis) : angle(_angle), axis(_axis) {};
    Rotation(const Rotation& other) : angle(other.angle), axis(other.axis) {};

    float angle;
    glm::vec3 axis;
};

struct Transform{
    Transform(const glm::vec3& _translation, const glm::vec3& _scale, const Rotation& _rotation) : 
        translation(_translation), scale(_scale), rotation(_rotation) {};

    Transform(const Transform& other) {
        translation = other.translation;
        scale = other.scale;
        rotation = other.rotation;
    }

    glm::mat4 world() const;

    glm::vec3 translation;
    glm::vec3 scale;
    Rotation rotation;
};

std::ostream& operator << (std::ostream& out, const glm::mat4& mat);
