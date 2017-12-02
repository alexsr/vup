#version 450

in vec2 pass_uv;

uniform sampler2D image;

void main() {
	gl_FragColor = texture(image, pass_uv);
}
