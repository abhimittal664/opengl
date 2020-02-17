#version 330 core										

struct vertexData{
	vec4 position;
	vec2 texCoord;
	vec4 normal;
};

struct ambientLight{
	vec4	color;
	float	intensity;
};

struct directionalLight{
	vec4	color;
	float	intensity;
	vec4	direction;
};

struct specularLight{
	vec4	color;
	float	intensity;
	vec4	position;
};

struct camera{
	vec4 cameraPos;
};

struct material{
	vec3		ambientCoef;
	vec3		diffuseCoef;
	vec3		specularCoef;
	vec3		emissiveCoef;
	float		specularFactor;
	int			usesAmbientTex;
	int			usesDiffuseTex;
	sampler2D	ambientTextureMap;
	sampler2D	diffuseTextureMap;
};

in vertexData vertex_data;

//Uniforms
uniform material				objectMaterial;
uniform camera					camera_0;
uniform ambientLight			ambientLight_0;
uniform directionalLight		directionalLight_0;
uniform specularLight			specularLight_0;

//----------------Function defs start-------------------
vec4 calcAmbientComponent(ambientLight ambientLightData){
	return (vec4(ambientLightData.color * ambientLightData.intensity));
}

vec4 calcDiffuseComponent(directionalLight directionalLightData){
	vec4 lightVec, normalVec, diffuseComponent;
	
	//Normalise
	lightVec	= normalize(directionalLightData.direction);
	normalVec	= normalize(vertex_data.normal);

	//Calculate final color
	diffuseComponent =	directionalLightData.color * directionalLightData.intensity *
						dot(lightVec, normalVec);
	diffuseComponent = max(diffuseComponent, 0.f);

	return (diffuseComponent);
}


vec4 calcSpecularComponent(specularLight specularLightData, camera cameraData){
	vec4 normalizedNormal	=	normalize(vertex_data.normal);
	vec4 normalizedPos		=	normalize(vertex_data.position);

	vec4 incidentLightVec	= normalizedPos - specularLightData.position;
	vec4 reflectionVec		= normalize(reflect(incidentLightVec, normalizedNormal));
	vec4 cameraPosVec		= normalize(cameraData.cameraPos - normalize(vertex_data.position));

	float angle = dot(reflectionVec, cameraPosVec);
	float specularFactor;
	if (angle > 0) {
		specularFactor = pow(angle, objectMaterial.specularFactor);
	}
	else {
		specularFactor = 0;
	}
	vec4 specularComponent	=	specularLightData.color * specularLightData.intensity * specularFactor;

	specularComponent		=	min(specularComponent, 1.f);

	return (specularComponent);
}


//----------------Funnction defs end--------------------

void main(){			
	vec4 ambientBase, diffuseBase, specularBase, emissiveBase; 
	vec4 ambientComponent, diffuseComponent, specularComponent, emissiveComponent;
	vec4 finalColor;
	vec4 posCameraSpace;

	posCameraSpace = gl_FragCoord;;

	if(objectMaterial.usesAmbientTex){
		ambientBase = texture(objectMaterial.ambientTextureMap, vertex_data.texCoord);
	} else {
		ambientBase = vec4(objectMaterial.ambientCoef, 1.f);
	}
	//Ambient Light
	ambientComponent = calcAmbientComponent(ambientLight_0);

	if(objectMaterial.usesDiffuseTex){
		diffuseBase = texture(objectMaterial.diffuseTextureMap, vertex_data.texCoord);
	} else{
		diffuseBase = vec4(objectMaterial.diffuseCoef, 1.f);
	}
	//Diffuse Light
	diffuseComponent = calcDiffuseComponent(directionalLight_0);

	//Point Light
	specularBase = vec4(objectMaterial.specularCoef, 1.f);
	specularComponent = calcSpecularComponent(specularLight_0, camera_0);

	//Emissive
	emissiveBase = vec4(objectMaterial.emissiveCoef, 1.f);


	finalColor =	ambientBase * ambientComponent +
					diffuseBase * diffuseComponent +
					specularBase * specularComponent +
					emissiveBase;

	//Assign final color
	gl_FragColor = (min(finalColor, 1.f));	
}