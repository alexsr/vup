#version 450 core

in vec3 pass_color;
in vec3 pass_position;

out vec4 fragColor;

void main() {
    vec2 diff = vec2(gl_PointCoord-0.5f)*2.0f;
    float squared_dist = dot(diff, diff);
    float radius = -(pass_position.z - 1.0f)/2.0f;
    if (squared_dist > radius) {
        discard;
    }
	fragColor = vec4(pass_color, 1.0f);
}
