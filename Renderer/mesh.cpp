#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>



model::model(const std::string& ModelPath, std::string& ModelFile)
	:	modelPath{ ModelPath},
		modelFile{ ModelFile }
{

	readModelFile((ModelPath + ModelFile).c_str());
}

//This function will read the model
void model::readModelFile(const std::string& ModelFile) {
	Assimp::Importer* importer = new Assimp::Importer();

	const aiScene* scene = importer->ReadFile(ModelFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenNormals);

	//Check for Errors
	if (!scene) {
		std::cout << importer->GetErrorString() << std::endl;
		return;
	}

	this->importer = importer;
	this->scene = scene;
}

//This function iterates over all meshes and builds VAO, VBO and IBOs
void model::buildBuffers() {
	
	//First get the meshes
	aiMesh*			modelMesh;
	aiMaterial*		material_obj;
	mesh*			mesh_obj;
	vertex_data		vertex;
	for (int n_mesh = 0; n_mesh < scene->mNumMeshes; ++n_mesh) {
		modelMesh = scene->mMeshes[n_mesh];
		material_obj = scene->mMaterials[modelMesh->mMaterialIndex];

		//Create new mesh and push to list
		mesh_obj = new mesh();
		
		//2. Get Face indices for index Buffer
		for (int n_face = 0; n_face < modelMesh->mNumFaces; ++n_face) {
			mesh_obj->indices.push_back(modelMesh->mFaces[n_face].mIndices[0]);
			mesh_obj->indices.push_back(modelMesh->mFaces[n_face].mIndices[1]);
			mesh_obj->indices.push_back(modelMesh->mFaces[n_face].mIndices[2]);
		}

		//3. Build packed structure for Vertex Attrributes
		for (int n_vertex = 0; n_vertex < modelMesh->mNumVertices; ++n_vertex) {
			vertex.position = glm::vec4(modelMesh->mVertices[n_vertex].x,
				modelMesh->mVertices[n_vertex].y,
				modelMesh->mVertices[n_vertex].z,
				1.f);

			vertex.normal = glm::vec3(modelMesh->mNormals[n_vertex].x,
				modelMesh->mNormals[n_vertex].y,
				modelMesh->mNormals[n_vertex].z);

			if (modelMesh->HasTextureCoords(n_mesh)) {
				vertex.texCoord = glm::vec2(modelMesh->mTextureCoords[0][n_vertex].x,
					modelMesh->mTextureCoords[0][n_vertex].y);
			}
			else {
				vertex.texCoord = glm::vec2(0.f, 0.f);
			}
			//vertex.texCoord = glm::vec2(1.f, 1.f);
			//Push vertex
			mesh_obj->vertices.push_back(vertex);
		}
		mesh_obj->materialIndex = modelMesh->mMaterialIndex;

		//Next get the textures and color Parameters
		aiColor3D color;
		aiString texturePath;
		aiString name;

		mesh_obj->materialIndex = modelMesh->mMaterialIndex;
		mesh_obj->meshMaterial = new materialInfo();
		material_obj->Get(AI_MATKEY_NAME, name);
		mesh_obj->meshMaterial->materialName = name.C_Str();
		
		material_obj->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mesh_obj->meshMaterial->ambientCoef = glm::vec3(color.r, color.g, color.b);
		material_obj->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mesh_obj->meshMaterial->diffuseCoef = glm::vec3(color.r, color.g, color.b);
		material_obj->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mesh_obj->meshMaterial->specularCoef = glm::vec3(color.r, color.g, color.b);
		material_obj->Get(AI_MATKEY_SHININESS, mesh_obj->meshMaterial->specularFactor);

		//Get Textures now
		if (material_obj->GetTextureCount(aiTextureType_AMBIENT) > 0) {
			mesh_obj->meshMaterial->usesAmbientTex = 1;
			material_obj->GetTexture(aiTextureType_AMBIENT, 0, &texturePath);

			//Create new texture
			mesh_obj->meshMaterial->ambientTextureMap = new texture();
			mesh_obj->meshMaterial->ambientTextureMap->path = modelPath + (std::string)texturePath.C_Str();
			mesh_obj->meshMaterial->ambientTextureMap->loadTexture(mesh_obj->meshMaterial->ambientTextureMap->path);
		}

		if (material_obj->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			mesh_obj->meshMaterial->usesDiffuseTex = 1;
			material_obj->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);

			//Create new texture
			mesh_obj->meshMaterial->diffuseTextureMap = new texture();
			mesh_obj->meshMaterial->diffuseTextureMap->path = modelPath + (std::string)texturePath.C_Str();
			mesh_obj->meshMaterial->diffuseTextureMap->loadTexture(mesh_obj->meshMaterial->diffuseTextureMap->path);
		}

		//Push Mesh pointer to list
		meshList.push_back(mesh_obj);
	}

	//Start building buffers
	//1.VAO
	glGenVertexArrays(1, &VAO);

	//2.Vertex  and Index Buffers
	GLuint vboObj, iboObj;
	for (int i = 0; i < meshList.size(); ++i) {
		VBO.push_back(vboObj);
		IBO.push_back(iboObj);
		//Vertex Buffer
		glGenBuffers(1, &VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)*meshList[i]->vertices.size(), &meshList[i]->vertices[0], GL_STATIC_DRAW);

		//Index Buffer
		glGenBuffers(1, &IBO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*meshList[i]->indices.size(), &meshList[i]->indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

//Create Shader
void createShader(GLuint Program, const char* code, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);

	const GLchar* shaderCode[1];
	shaderCode[0] = code;

	GLint codeLength[1];
	codeLength[0] = strlen(code);

	glShaderSource(shader, 1, shaderCode, codeLength);
	glCompileShader(shader);

	GLint shaderResult = 0;
	GLchar log[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderResult);
	if (!shaderResult) {
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);
		std::cout << "Error Compiling Shader" << shaderType << " " << log << " " << std::endl;
		return;
	}

	glAttachShader(Program, shader);
}

//Create Shader program
void model::compileShader(const char* fileLocVS, const char* fileLocFS) {
	this->shaderProgram = glCreateProgram();

	if (!shaderProgram) {
		std::cout << "Shader Program Creation Failed" << std::endl;
		return;
	}

	//Load Shader Code
	std::string shaderCodeVS = loadShader(fileLocVS);
	std::string shaderCodeFS = loadShader(fileLocFS);

	//Create Shader
	createShader(shaderProgram, shaderCodeVS.c_str(), GL_VERTEX_SHADER);
	createShader(shaderProgram, shaderCodeFS.c_str(), GL_FRAGMENT_SHADER);

	GLint shaderResult = 0;
	GLchar log[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderResult);
	if (!shaderResult) {
		glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
		std::cout << "Error linking Program " << log << " " << std::endl;
		return;
	}

	//Validate the shader
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &shaderResult);
	if (!shaderResult) {
		glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);
		std::cout << "Error Validating Program " << log << " " << std::endl;
		return;
	}
}


//Load Shader
std::string model::loadShader(const char* shaderFileLoc) {
	std::string shaderContent;
	std::ifstream loc(shaderFileLoc, std::ios::in);

	//Check if file opened
	if (!loc) {
		std::cout << "Error opening shader at " << shaderFileLoc << std::endl;
		return "";
	}

	std::string line = "";
	while (!loc.eof())
	{
		std::getline(loc, line);
		shaderContent.append(line + "\n");
	}

	return shaderContent;

}

void model::getUniformLocs(GLuint shaderProgram, int meshIndex) {
	
	ambientCoefUniform = glGetUniformLocation(shaderProgram, "objectMaterial.ambientCoef");
	diffuseCoefUniform = glGetUniformLocation(shaderProgram, "objectMaterial.diffuseCoef");
	specularCoefUniform = glGetUniformLocation(shaderProgram, "objectMaterial.specularCoef");
	emissiveCoefUniform = glGetUniformLocation(shaderProgram, "objectMaterial.emissiveCoef");
	specularFactorUniform = glGetUniformLocation(shaderProgram, "objectMaterial.specularFactor");
	usesAmbientTexUniform = glGetUniformLocation(shaderProgram, "objectMaterial.usesAmbientTex");
	usesDiffuseTexUniform = glGetUniformLocation(shaderProgram, "objectMaterial.usesDiffuseTex");
	ambientTextureMapUniform = glGetUniformLocation(shaderProgram, "objectMaterial.ambientTextureMap");
	diffuseTextureMapUniform = glGetUniformLocation(shaderProgram, "objectMaterial.diffuseTextureMap");

}

//1 Mesh will only have 1 material
void model::activateMaterial(GLuint shaderProgram, int meshIndex) {

	glUniform3f(ambientCoefUniform, this->meshList[meshIndex]->meshMaterial->ambientCoef.x, this->meshList[meshIndex]->meshMaterial->ambientCoef.y, this->meshList[meshIndex]->meshMaterial->ambientCoef.z);
	glUniform3f(diffuseCoefUniform, this->meshList[meshIndex]->meshMaterial->diffuseCoef.x, this->meshList[meshIndex]->meshMaterial->diffuseCoef.y, this->meshList[meshIndex]->meshMaterial->diffuseCoef.z);
	glUniform3f(specularCoefUniform, this->meshList[meshIndex]->meshMaterial->specularCoef.x, this->meshList[meshIndex]->meshMaterial->specularCoef.y, this->meshList[meshIndex]->meshMaterial->specularCoef.z);
	glUniform3f(emissiveCoefUniform, this->meshList[meshIndex]->meshMaterial->emissiveCoef.x, this->meshList[meshIndex]->meshMaterial->emissiveCoef.y, this->meshList[meshIndex]->meshMaterial->emissiveCoef.z);
	glUniform1f(specularFactorUniform, this->meshList[meshIndex]->meshMaterial->specularFactor);
	glUniform1i(usesAmbientTexUniform, this->meshList[meshIndex]->meshMaterial->usesAmbientTex);
	glUniform1i(usesDiffuseTexUniform, this->meshList[meshIndex]->meshMaterial->usesDiffuseTex);

	//Bind Textures here
	glUniform1i(ambientTextureMapUniform, 0);
	glUniform1i(diffuseTextureMapUniform, 1);

	//Activate textures
	if (this->meshList[meshIndex]->meshMaterial->usesAmbientTex) {
		this->meshList[meshIndex]->meshMaterial->ambientTextureMap->useTexture(0);
	}
	if (this->meshList[meshIndex]->meshMaterial->usesDiffuseTex) {
		this->meshList[meshIndex]->meshMaterial->diffuseTextureMap->useTexture(1);
	}
}

//Render meshes 1 by 1
void model::renderMesh(GLuint shaderProgram) {
	for (int i = 0; i < meshList.size(); ++i) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[i]);

		//Layout
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_data), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)offsetof(vertex_data, texCoord));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_data), (void*)offsetof(vertex_data, normal));
		
		//Enable Attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//Get uniforms
		getUniformLocs(shaderProgram, i);

		//Bind Material Uniforms
		activateMaterial(shaderProgram, i);

		//Draw
		glDrawElements(GL_TRIANGLES, meshList[i]->indices.size(), GL_UNSIGNED_INT, 0);

		//Unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void model::clearModelData() {
	for (int i = 0; i < VBO.size(); ++i) {
		glDeleteBuffers(1, &VBO[i]);
	}
	for (int i = 0; i < IBO.size(); ++i) {
		glDeleteBuffers(1, &IBO[i]);
	}
	for (int i = 0; i < meshList.size(); ++i) {
		meshList[i] = NULL;
	}
}

model::~model()
{
	clearModelData();
}
