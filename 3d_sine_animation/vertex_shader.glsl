//Create  vertex shader

#version 430
layout (location = 0) in	vec3 vp;
layout (location = 1) in	vec3 vc;
					  out	vec4 vc_vs;
					  uniform mat4 MVP;
					  uniform float time;
					  const float amplitude = 0.5;
					  const float frequency = 4;
					  const float PI		= 3.14f;
void main() {
float distance	= length(vp);
float y 		= amplitude*sin(2*PI*distance*frequency*time); 
gl_Position = MVP*vec4 (vp.x, y, vp.z, 1.0f);
};