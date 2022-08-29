#version 460 core

in vec2 tex_coords;
out vec4 frag_color;

uniform sampler2DArray texture_0;

uniform int current_frame;
uniform int total_frames;

float actual_layer() {
    return max(0, min(total_frames - 1,
                      floor(current_frame + 0.5)));
}

void main() {
    //frag_color = vec4(0.1, 0.4, 0.1, 1.0);
    frag_color = texture(texture_0, vec3(tex_coords, actual_layer()));
}
