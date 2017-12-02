#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;

layout (std140, binding = 9) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec4 pass_position;
out vec4 pass_normal;
out vec2 pass_uv;

void main() {
    pass_normal = transpose(inverse(view * model)) * normal;
    pass_uv = uv;
    pass_position = projection * view * model * position;
	gl_Position = pass_position;
}