#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "sph_particle.inc.comp"

layout (std430, binding = 8) buffer particles {
    SPH_particle p[N];
};

layout (std140, binding = 9) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;

void main() {
    pass_normal = vec3(transpose(inverse(view * model)) * normal);
//    pass_normal = vec3(0, p[gl_InstanceID].density / 1000.0f, 0);
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
