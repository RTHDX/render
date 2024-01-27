#version 460 core

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_uv;

layout(location = 2) in mat4 ins_model;
layout(location = 6) in float ins_tile_index;

uniform mat4 projection;
uniform mat4 view;

out vec2 uv;
flat out float tile_index;

void main() {
    uv = in_uv;
    tile_index = ins_tile_index;
    gl_Position = projection * view * ins_model * vec4(in_pos, 1.0);
}
