//Create  vertex shader

#version 430
layout (location = 0) in	vec3 vp;
layout (location = 1) in	vec3 vc;
					  out	vec4 vc_vs;
					  uniform mat4 MVP;
					  const float amplitude = 0.5;
					  const float frequency = 4;
					  const float PI		= 3.14f;
void main() {
float distance	= length(vp);
float y_c			= vp.x; 
gl_Position = MVP*vec4 (vp.x, y_c, 0.f, 1.0f);
if(vp.x > 0){
	vc_vs = vec4(1.0, 0, 0, 1.0);
} else {
	vc_vs		= vec4(vc, 1.0);
	}
};