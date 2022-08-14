#version 460 core

in vec2 tex_coords;
out vec4 frag_color;

uniform sampler2D texture_0;

void main() {
    //frag_color = vec4(0.1, 0.4, 0.1, 1.0);
    frag_color = texture(texture_0, tex_coords);
}
