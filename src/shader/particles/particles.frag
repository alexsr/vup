#version 450

in vec4 pass_position;
in vec3 pass_normal;
in vec4 pass_color;

out vec4 frag_color;

uniform vec4 light_pos;

void main() {
	vec4 ambient_light = vec4(0.4, 0.4, 0.4, 1.0);
	vec3 light_vec = normalize(light_pos.xyz - pass_position.xyz);
	float cos_phi = max(dot(pass_normal, light_vec), 0.0f);
	frag_color = pass_color * ambient_light + vec4(vec3(pass_color * cos_phi), 0.0f);
}
