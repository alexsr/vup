#version 450

in vec3 pass_normal;

out vec4 frag_color;

void main() {
	frag_color = vec4(pass_normal, 1);
}
