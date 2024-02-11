#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform mat4 view;
uniform mat4 projection;

layout (location = 2) in mat4 in_model;

out vec2 uv;


void main() {
    gl_Position = projection * view * in_model * vec4(in_position, 1.0);
    uv = in_uv;
}
