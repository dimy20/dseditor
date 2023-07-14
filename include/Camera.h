#pragma once

#include <glm/glm.hpp>


struct Camera{
    Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);

    void update();
    void update_mouse_movement(float x_offset, float y_offset);
    glm::mat4 view() const;

    glm::vec3 m_position;
    glm::vec3 m_front_direction;
    glm::vec3 m_up;
    glm::vec3 m_right;
    float m_speed;
    float m_yaw;
    float m_pitch;
    float m_sens;
};

//voi

