#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

layout (std140, binding = 9) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (std430, binding = 5) buffer mesh {
    vec4 mesh_verts[];
};

out vec3 pass_normal;

void main() {
    pass_normal = vec3(transpose(inverse(view * model)) * normal);
    vec4 new_pos = view * model * position;
    mesh_verts[gl_VertexID] = new_pos;
	gl_Position = projection * view * model * position;
}
