#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coord;

uniform mat4 MVP;

out vec3 vertex_color;
out vec2 texture_coord;

void main(){
    gl_Position = MVP * vec4(aPos, 1.0);
    texture_coord = in_texture_coord;
}
