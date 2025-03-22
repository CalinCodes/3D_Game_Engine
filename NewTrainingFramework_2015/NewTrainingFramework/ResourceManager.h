#pragma once
#include "../Utilities/Math.h"
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include "Vertex.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "Shaders.h"

struct ModelResource {
	std::string file;
};

struct TextureResource {
	std::string file;
	int min_filter, mag_filter, wrap_s, wrap_t;
};

struct ShaderResource {
	std::string fileVS, fileFS;
};

class Model
{
public:
	ModelResource *mr;
	GLuint iboId, wiredIboId,vboId;
	GLsizei indexCount, wiredIndexCount;
	Model();
	~Model();
	void Load();
};

class Texture
{
public:
	TextureResource *tr;
	GLuint textureId;
	Texture();
	~Texture();
	void Load();
};

class Shader
{
public:
	ShaderResource *sr;
	GLuint program, vertexShader, fragmentShader;
	GLint positionAttribute, colorAttribute, matrixUniform, MVP, textureUniform, uvAttribute;
	Shader();
	~Shader();
	void Load();
};

class ResourceManager
{
private:
	static ResourceManager* spInstance;
	ResourceManager();
public:
	void Init();//initializari - pot fi si in constructor
	static ResourceManager* getInstance();
	void freeResources();//eliberarea zonelor de memorie alocate dinamic - se poate realiza si in destructor
	~ResourceManager();

	std::map<int, ModelResource*> modelResources;
	std::map<int, TextureResource*> textureResources;
	std::map<int, ShaderResource*> shaderResources;

	std::map<int, Model*> loadedModels;
	std::map<int, Texture*> loadedTextures;
	std::map<int, Shader*> loadedShaders;

	Model* loadModel(int id);
	Texture* loadTexture(int id);
	Shader* loadShader(int id);

};


