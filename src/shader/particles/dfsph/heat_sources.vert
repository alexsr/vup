#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "heat_sources.inc.comp"

layout (std140, binding = 0) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec4 pass_position;
out vec3 pass_normal;
out vec4 pass_color;

vec3 clamped_mix(vec3 a, vec3 b, float t) {
    t = clamp(t, 0, 1);
    return a * (1 - t) + b * t;
}

vec3 hsv_to_rgb(vec3 hsv) {
    float c = hsv.y * hsv.z;
    float h = hsv.x / 60.0f;
    float x = c * (1.0f - abs(mod(h, 2.0f) - 1.0f));
    float m = hsv.z - c;
    vec3 res = vec3(m, m, m);
    if (h < 0 || isnan(h)) {
        return res;
    }
    else if(h < 1) {
        res += vec3(c, x, 0);
    }
    else if (h < 2) {
        res += vec3(x, c, 0);
    }
    else if (h < 3) {
        res += vec3(0, c, x);
    }
    else if (h < 4) {
        res += vec3(0, x, c);
    }
    else if (h < 5) {
        res += vec3(x, 0, c);
    }
    else if (h < 6) {
        res += vec3(c, 0, x);
    }
    return res;
}

void main() {
    vec4 pos = heat_sources[gl_InstanceID].pos;
    pass_color = vec4(hsv_to_rgb(clamped_mix(vec3(240.0f, 1, 1), vec3(0, 1, 1), heat_sources[gl_InstanceID].wattage / 6000.0f)), 1.0f);
	vec4(heat_sources[gl_InstanceID].wattage / 6000.0f, 0.2, 0.2, 1.0f);
    pass_normal = vec3(normal);
	pass_position = projection * view * vec4((pos.w * 0.1f * position + heat_sources[gl_InstanceID].pos).xyz, 1.0);
	gl_Position = pass_position;
}
