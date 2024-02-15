#version 460 core

in vec2 uv;
flat in float tile_index;

out vec4 FragColor;

uniform sampler2DArray tex_array;

void main() {
    vec3 atlasCoord = vec3(uv, tile_index);
    FragColor = texture(tex_array, atlasCoord);
}
