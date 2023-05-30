#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 a_texture_coord;

out vec3 vertex_color;
out vec2 texture_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    //vertex_color = a_color;
	texture_coord = a_texture_coord;
}
