#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex;

uniform mat4 projection;
uniform mat4 view;

layout (location = 2) in mat4 in_model;

out vec2 frag_tex;


void main() {
    gl_Position = projection * view * in_model * vec4(in_pos, 1.0);
    frag_tex = in_tex;
}
