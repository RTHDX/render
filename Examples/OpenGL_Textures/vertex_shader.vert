#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 tex_pos;

out vec2 tex_coords;

void main() {
    gl_Position = vec4(vert_pos, 1.0);
    tex_coords = tex_pos;
}
