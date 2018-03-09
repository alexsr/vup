#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "particles.inc.comp"

layout (std430, binding = 0) buffer particles {
    Fluid_particle p[N];
};

layout (std140, binding = 8) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;

void main() {
    pass_normal = vec3(transpose(inverse(view * model)) * normal);
    //pass_normal = vec3(0, p[gl_InstanceID].pressure / 1.0f, 0);
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
