#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "particle_util.inc.comp"

#include "/../../data_structures/uniform_grid.inc.comp"

layout (std430, binding = 0) buffer particles {
    Fluid_particle p[];
};

layout (std430, binding = 6) buffer grid_parameters {
    Fixed_uniform_grid_params grid_params;
};

layout (std140, binding = 8) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;

void main() {
    vec4 pos = p[gl_InstanceID].pos;
    float i = (pos.x - grid_params.grid_min.x) / grid_params.cell_size.x;
    float j = (pos.y - grid_params.grid_min.y) / grid_params.cell_size.y;
    float k = (pos.z - grid_params.grid_min.z) / grid_params.cell_size.z;
    pass_normal = vec3(normal);//vec3(i / grid_params.cell_count.x,
                    //    j / grid_params.cell_count.y,
                    //    k / grid_params.cell_count.z);
    //pass_normal = vec3(0, p[gl_InstanceID].pressure / 1.0f, 0);
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
