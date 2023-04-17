#version 460 core
layout (location = 0) in vec3 in_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 world_position;

void main()
{
    world_position = (model * vec4(in_pos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(in_pos, 1.0);
}
