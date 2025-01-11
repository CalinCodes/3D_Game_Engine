#pragma once
#include "../Utilities/Math.h"
#include <map>
#include <string>

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
	GLuint program;
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

	std::map<int, ModelResource*> models;
	std::map<int, TextureResource*> textures;
	std::map<int, ShaderResource*> shaders;

	std::map<int, ModelResource*> loadedModels;
	std::map<int, TextureResource*> loadedTextures;
	std::map<int, ShaderResource*> loadedShaders;

	void loadModel(Model model);
	void loadTexture(Texture texture);
	void loadShader(Shader shader);

};


