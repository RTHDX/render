#version 330 core

in vec2 uv;
out vec4 out_color;

uniform sampler2D tex;

void main() {
    vec4 p = texture(tex, uv);
    out_color = p;
}
