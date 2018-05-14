#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

#include "particle_util.inc.comp"

layout (std430, binding = 0) buffer particles {
    Particle p[];
};

layout (std140, binding = 0) uniform mvp {
    mat4 model;
    mat4 view;
    mat4 projection;
};

out vec3 pass_normal;

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
    vec4 pos = p[gl_InstanceID].pos;
    pass_normal = hsv_to_rgb(clamped_mix(vec3(240.0f, 1, 1), vec3(0, 1, 1), (float(p[gl_InstanceID].temperature) - 273.15f)/100.0f));
    // pass_normal = vec3(normal);
	gl_Position = projection * view * model * vec4((position + p[gl_InstanceID].pos).xyz, 1.0);
}
