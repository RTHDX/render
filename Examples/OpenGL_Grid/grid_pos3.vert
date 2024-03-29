#version 460 core

layout(location = 0) in vec3 in_pos;

layout(location = 1) out vec3 near_point;
layout(location = 2) out vec3 far_point;

uniform mat4 projection;
uniform mat4 view;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = in_pos;
    // unprojecting on the near plane
    near_point = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz;
    // unprojecting on the far plane
    far_point = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz;
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}
