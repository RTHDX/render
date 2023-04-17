#version 460 core
out vec4 out_color;

in vec3 world_position;

uniform vec3 grid_color;
uniform float grid_size;
uniform float line_width;

void main() {
    vec2 grid_position = mod(world_position.xz, grid_size);
    float line = min(grid_position.x, grid_position.y);

    if (line < line_width)
        out_color = vec4(grid_color, 0.5);
    else
        discard;
}
