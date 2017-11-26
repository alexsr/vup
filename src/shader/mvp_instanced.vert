#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec2 offset;
layout (location = 4) in vec3 color;

out vec3 pass_color;

uniform mat4 view;
uniform mat4 proj;

void main() {
    pass_color = color;
	gl_Position = proj * view * (position + vec4(offset, 0.0f, 0.0f));
}
