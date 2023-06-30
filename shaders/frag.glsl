#version 330 core

out vec4 frag_color;
in vec2 texture_coord;
uniform sampler2D my_texture;

void main(){
    frag_color = vec4(1.0, 0, 0, 1.0);
    //frag_color = texture(my_texture, texture_coord);

}
