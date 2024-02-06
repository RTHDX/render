#version 460 core

in vec2 uv;
out vec4 out_color;

uniform sampler2D texture_sampler;


void main() {
    out_color = texture(texture_sampler, uv);
}
