#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;

layout (std140, binding = 0) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;

void main() {
    pass_normal = vec3(transpose(inverse(view * model)) * normal);
	gl_Position = projection * view * model * position;
}
