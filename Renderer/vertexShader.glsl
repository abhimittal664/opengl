#version 330 core										

struct vertexData{
	vec4 position;
	vec2 texCoord;
	vec4 normal;
};

uniform mat4 model;	
uniform mat4 view;	
uniform mat4 projection;

										
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vertexData vertex_data;

void main(){	
	mat4 normalTransform	= transpose(inverse(view));
	
	gl_Position				= normalize(projection*view*position);		
	
	//Need to transform normal to camera space

	//Assign outputs
	vertex_data.position	= normalize(view*position);
	vertex_data.texCoord	= (texCoord);
	vertex_data.normal		= normalize(normalTransform*vec4(normal, 1.f));
	
}