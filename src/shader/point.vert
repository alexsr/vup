#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 pass_color;
out vec3 pass_position;

void main() {
    pass_color = color;
    pass_position = position;
	gl_Position = vec4(position, 1.0f);
}
