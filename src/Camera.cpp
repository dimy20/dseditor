#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "RenderContext.h"
#include "input.h"

static int prev_mouse_x = 0;
static int prev_mouse_y = 0;
static bool first_time = true;

#define DEFAULT_CAMERA_SPEED 10.0f
Camera::Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up) : 
        m_position(position),
        m_front_direction(front),
        m_up(up),
        m_speed(DEFAULT_CAMERA_SPEED),
        m_yaw(-90.0f),
        m_pitch(0.0), // ?
        m_sens(0.1f){};

glm::mat4 Camera::view() const{ return glm::lookAt(m_position, m_position + m_front_direction, m_up); };

void Camera::update_mouse_movement(float x_offset, float y_offset){
    float yaw_offset = x_offset * m_sens;
    float pitch_offset = y_offset * m_sens; 

    m_yaw += yaw_offset;
    m_pitch += pitch_offset;

    if(m_pitch > 89.0f) m_pitch = 89.0f;
    if(m_pitch < -89.0f) m_pitch = -89.0f;

    glm::vec3 new_direction;

    new_direction.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    new_direction.y = std::sin(glm::radians(m_pitch));
    new_direction.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

    m_front_direction = glm::normalize(new_direction);
};

void Camera::update(){
    if(RobyInput::is_key_pressed(RobyInput::Key::D))
        m_position -= glm::normalize(glm::cross(m_up, m_front_direction)) * m_speed * RobyRender::delta_time;

    if(RobyInput::is_key_pressed(RobyInput::Key::A))
        m_position += glm::normalize(glm::cross(m_up, m_front_direction)) * m_speed * RobyRender::delta_time;

    if(RobyInput::is_key_pressed(RobyInput::Key::W))
        m_position += m_front_direction * m_speed * RobyRender::delta_time;

    if(RobyInput::is_key_pressed(RobyInput::Key::S))
        m_position -= m_front_direction * m_speed * RobyRender::delta_time;

    if(first_time){
        prev_mouse_x = RobyInput::mouse_x;
        prev_mouse_y = RobyInput::mouse_y;
        first_time = false;
    }

    float x_offset = (RobyInput::mouse_x - prev_mouse_x);
    float y_offset = (prev_mouse_y - RobyInput::mouse_y);

    prev_mouse_x = RobyInput::mouse_x;
    prev_mouse_y = RobyInput::mouse_y;

    update_mouse_movement(x_offset, y_offset);
}
