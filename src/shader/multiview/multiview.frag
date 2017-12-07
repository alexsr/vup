#version 450

in vec4 pass_color;

out vec4 frag_color;

void main() {
	frag_color = pass_color;
}
