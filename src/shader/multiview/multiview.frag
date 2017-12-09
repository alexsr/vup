#version 450

in vec4 pass_color;

out vec4 frag_color;

void main() {
	frag_color = gl_FrontFacing ? pass_color : vec4(0,0,0,1);
}
