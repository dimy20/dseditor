#version 330 core

out vec4 frag_color;
in vec2 texture_coord;

uniform sampler2D texture_sampler;

void main(){
    frag_color = texture(texture_sampler, texture_coord);
}
