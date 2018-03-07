#version 450

in vec4 pass_position;
in vec4 pass_normal;
in vec2 pass_uv;

layout (location = 0) out vec4 p;
layout (location = 1) out vec4 n;
layout (location = 2) out vec4 u;

void main() {
    p = pass_position;
    n = pass_normal;
    u = vec4(pass_uv, 1.0, 1.0);
}
