#version 460 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_tex;

out vec2 uv;

void main() {
    gl_Position = vec4(in_pos, 1.0);
    uv = in_tex;
}