#version 460 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D texture_1;


void main() {
    frag_color = vec4(1.0, 1.0, 1.0, 1.0);
}
