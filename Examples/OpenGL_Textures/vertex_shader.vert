#version 460 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_norm;
layout (location = 2) in vec2 tex_pos;

out vec3 out_normal;
out vec2 tex_coord;

void main() {
    gl_Position = vec4(vert_pos, 1.0);
    tex_coord = tex_pos;
    out_normal = vert_normal;
}
