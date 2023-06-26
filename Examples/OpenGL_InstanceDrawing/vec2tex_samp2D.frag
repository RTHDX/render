#version 460 core

in vec2 frag_tex;
out vec4 frag_color;

uniform sampler2D texture_1;

void main() {
    frag_color = texture(texture_1, frag_tex);
}