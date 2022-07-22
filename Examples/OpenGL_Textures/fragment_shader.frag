#version 460 core

in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D texture_1;

void main() {
    //frag_color = vec4(0.3, 0.8, 0.8, 1.0);
    frag_color = texture(texture_1, tex_coords);
}
