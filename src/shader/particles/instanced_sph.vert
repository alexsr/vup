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
    pass_normal.r = 0;
    if (p[gl_InstanceID].rest_density < 1200) {
        pass_normal.r = 1;
    }
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
