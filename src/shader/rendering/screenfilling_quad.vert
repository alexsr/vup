#version 450

layout (location = 0) in vec4 position;
layout (location = 2) in vec2 uv;

out vec2 pass_uv;

void main() {
    gl_Position = position;
    pass_uv = uv;
}
