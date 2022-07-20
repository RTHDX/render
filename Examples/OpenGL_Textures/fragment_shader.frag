#version 460 core

in vec3 vert_normal;
in vec2 tex_coords;

out vec4 frag_color;

uniform sampler2D texture_1;

void main() {
    //frag_color = vec4(1.0, 1.0, 1.0, 1.0);
    frag_color = texture(texture_1, tex_coords);
}
