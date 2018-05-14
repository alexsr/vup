#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "particle_util.inc.comp"

#include "boundary.inc.comp"

layout (std140, binding = 0) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;
void main() {
    vec4 pos = boundaries[gl_InstanceID].pos;
    pass_normal = vec3(pos.w);
	gl_Position = projection * view * model * vec4((position + pos).xyz, 1.0);
}
