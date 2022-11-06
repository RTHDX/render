#version 460 core

in vec3 position;
in vec3 normal;

out vec4 FragColor;

uniform vec4 color;

uniform vec4 light_color;
uniform vec3 light_position;


vec4 eval_ambient_color() {
    float intencity = 0.8;
    return intencity * light_color;
}

vec4 eval_diffuse_color() {
    vec3 light_dir = normalize(light_position - position);
    float diff = max(dot(normal, light_dir), 0.0);
    return diff * light_color;
}

void main() {
    vec4 ambient = eval_ambient_color();
    vec4 diffuse = eval_diffuse_color();
    FragColor = (ambient + diffuse) * color;
};
