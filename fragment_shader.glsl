//Create fragment shader
#version 430
in  vec4 vc_vs;
out vec4 fragment_color;
void main() {
	fragment_color = vc_vs;
};
