#version 450

layout (location = 0) in vec4 position;

struct MVP_mats {
    mat4 model;
    mat4 view;
    mat4 projection;
};

layout (std140, binding = 9) uniform mats {
    MVP_mats mvp[N];
};

void main() {
	gl_Position = mvp[0].projection * mvp[gl_InstanceID].view * position;
}
