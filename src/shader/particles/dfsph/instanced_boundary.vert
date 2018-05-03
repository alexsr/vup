#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "particle_util.inc.comp"

#include "/../../data_structures/uniform_grid.inc.comp"

layout (std430, binding = 9) buffer particles {
    vec4 p[];
};

layout (std140, binding = 8) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;
void main() {
    vec4 pos = p[gl_InstanceID];
    pass_normal = vec3(pos.w);
	gl_Position = projection * view * model * vec4((position + pos).xyz, 1.0);
}
