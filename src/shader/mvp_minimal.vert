#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 view;
uniform mat4 proj;

out vec4 pass_color;

void main() {
    pass_color = vec4(uv, 0, 1);
	gl_Position = proj * view * position;
}
