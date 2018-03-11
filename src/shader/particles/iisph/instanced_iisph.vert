#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

struct Fluid_particle {
    vec4 pos;
    vec4 vel;
    vec4 vel_adv;
    vec4 pressure_accel;
    vec4 dii;
    vec4 dij_pj_sum;
    float mass;
    float rest_density;
    float density;
    float density_adv;
    float pressure;
    float last_pressure;
    float aii;
    float aij_pj_sum;
};

struct Fixed_uniform_grid_params {
    vec4 grid_min;
    ivec4 cell_count;
    vec4 cell_size;
    vec4 grid_pos;
    int cell_capacity;
    int total_cell_count;
    int grid_capacity;
};

layout (std430, binding = 0) buffer particles {
    Fluid_particle p[N];
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
    pass_normal = vec3(i / grid_params.cell_count.x,
                       j / grid_params.cell_count.y,
                       k / grid_params.cell_count.z);
    //pass_normal = vec3(0, p[gl_InstanceID].pressure / 1.0f, 0);
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
