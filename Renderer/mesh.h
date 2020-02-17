#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <map>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "texture.h"

//Struct def for shaders
typedef struct {
	glm::vec4	position;
	glm::vec2	texCoord;
	glm::vec3	normal;
} vertex_data;

//Only supports ambient and Diffuse Texture Maps for now
 class materialInfo {	
 public:
	const char*		materialName;
	glm::vec3		ambientCoef;
	glm::vec3		diffuseCoef;
	glm::vec3		specularCoef;
	glm::vec3		emissiveCoef;
	float			specularFactor;
	int				usesAmbientTex;
	int				usesDiffuseTex;
	texture*		ambientTextureMap;
	texture*		diffuseTextureMap;
} ;

class mesh
{
public:
	std::vector<vertex_data>	vertices;	//Packed structure
	std::vector<GLuint>			indices;
	GLuint						materialIndex;
	materialInfo*				meshMaterial;


	
};



class model {
public:
	model(const std::string& ModelPath, std::string& ModelFile);
	~model();

	//Public functions
	void									buildBuffers();
	void									loadTextures(mesh* currentMesh);
	std::string								loadShader(const char*);
	void									compileShader(const char*, const char*);
	void									getUniformLocs(GLuint shaderProgram, int meshIndex);
	void									activateMaterial(GLuint shaderProgram, int meshIndex);
	void									renderMesh(GLuint shaderProgram);
	void									clearModelData();

	//Public variables
	std::vector<mesh*>		meshList;
	GLuint					VAO;
	std::vector<GLuint>		VBO, IBO;
	GLuint					shaderProgram;
	GLuint					uniformModel, uniformView, uniformProjection;

	//Uniforms
	GLuint						ambientCoefUniform,
								diffuseCoefUniform,
								specularCoefUniform,
								emissiveCoefUniform,
								specularFactorUniform,
								usesAmbientTexUniform,
								usesDiffuseTexUniform,
								ambientTextureMapUniform,
								diffuseTextureMapUniform;

private:
	Assimp::Importer*		importer;
	const aiScene*			scene;
	std::string				modelPath, modelFile;
	const char*				materialFile;
	void					readModelFile(const std::string& ModelFile);

};
