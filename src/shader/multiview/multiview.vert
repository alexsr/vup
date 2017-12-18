#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec4 color;

struct MVP_mats {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (std140, binding = 9) uniform mats {
    MVP_mats mvp[N];
};

out vec4 pass_color;
flat out int id;

void main() {
    id = gl_InstanceID;
    pass_color = color;
	gl_Position = mvp[id].projection * mvp[id].view * mvp[id].model * position;
}
