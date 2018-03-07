#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 pass_normal;

void main() {
    pass_normal = vec3(transpose(inverse(view * model)) * vec4(normal, 0));
	gl_Position = proj * view * model * position;
}
