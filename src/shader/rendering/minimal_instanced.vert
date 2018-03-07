#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec2 offset;
layout (location = 4) in vec3 color;

out vec3 pass_color;

void main() {
    pass_color = color;
	gl_Position = position + vec4(offset, 0, 0);
}
