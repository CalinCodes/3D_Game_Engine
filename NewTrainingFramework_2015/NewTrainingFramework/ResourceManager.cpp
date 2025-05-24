#include "stdafx.h"
#include "ResourceManager.h"
#include "../Utilities/TGA.cpp"

void readNfg(std::string nfgPath, std::vector<Vertex>& vertexVector, std::vector<unsigned short>& indexVector);
char* LoadTGA(const char* szFileName, int* width, int* height, int* bpp);

ResourceManager* ResourceManager::spInstance = NULL;

ResourceManager::ResourceManager()
{

}

void ResourceManager::Init()
{
	rapidxml::file<> file("../../XML/resourceManager.xml");
	char* buffer = new char[file.size() + 1];
	std::memcpy(buffer, file.data(), file.size());
	buffer[file.size()] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);
	rapidxml::xml_node<>* root = doc.first_node("resourceManager");
	char Res[] = "../../NewResourcesPacket";
	for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {
		if (strcmp(node->name(), "models") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* model = folder->first_node("model");
				while (model)
				{
					ModelResource* mr = new ModelResource();
					mr->file = Res + path.substr(path.find('/')) + model->first_node("file")->value();
					modelResources.insert(std::pair<int, ModelResource*>(std::stoi(model->first_attribute("id")->value()), mr));
					model = model->next_sibling();
				}
			}
		}
		else if (strcmp(node->name(), "textures") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* texture = folder->first_node("texture");
				while (texture)
				{
					TextureResource* tr = new TextureResource();

					tr->file = Res + path.substr(path.find('/')) + texture->first_node("file")->value();

					if (strcmp(texture->first_node("min_filter")->value(), "LINEAR") == 0)
					{
						tr->min_filter = GL_LINEAR;
					}
					else if (strcmp(texture->first_node("min_filter")->value(), "NEAREST") == 0)
					{
						tr->min_filter = GL_NEAREST;
					}

					if (strcmp(texture->first_node("mag_filter")->value(), "LINEAR") == 0)
					{
						tr->mag_filter = GL_LINEAR;
					}
					else if (strcmp(texture->first_node("mag_filter")->value(), "NEAREST") == 0)
					{
						tr->mag_filter = GL_NEAREST;
					}

					if (strcmp(texture->first_node("wrap_s")->value(), "CLAMP_TO_EDGE") == 0)
					{
						tr->wrap_s = GL_CLAMP_TO_EDGE;
					}
					else if (strcmp(texture->first_node("wrap_s")->value(), "REPEAT") == 0)
					{
						tr->wrap_s = GL_REPEAT;
					}

					if (strcmp(texture->first_node("wrap_t")->value(), "CLAMP_TO_EDGE") == 0)
					{
						tr->wrap_t = GL_CLAMP_TO_EDGE;
					}
					else if (strcmp(texture->first_node("wrap_t")->value(), "REPEAT") == 0)
					{
						tr->wrap_t = GL_REPEAT;
					}

					if (texture->first_node("type"))
					{
						tr->type = texture->first_node("type")->value();
					}

					textureResources.insert(std::pair<int, TextureResource*>(std::stoi(texture->first_attribute("id")->value()), tr));
					texture = texture->next_sibling();
				}
			}
		}
		else if (strcmp(node->name(), "shaders") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("folder"); folder; folder = folder->next_sibling())
			{
				std::string path = folder->first_attribute("path")->value();
				rapidxml::xml_node<>* shader = folder->first_node("shader");
				while (shader)
				{
					ShaderResource* sr = new ShaderResource();
					sr->fileVS = Res + path.substr(path.find('/')) + shader->first_node("vs")->value();
					sr->fileFS = Res + path.substr(path.find('/')) + shader->first_node("fs")->value();
					shaderResources.insert(std::pair<int, ShaderResource*>(std::stoi(shader->first_attribute("id")->value()), sr));

					shader = shader->next_sibling();
				}
			}
		}
	}
	delete[] buffer;
}

ResourceManager* ResourceManager::getInstance()
{
	if (!spInstance)
	{
		spInstance = new ResourceManager();
	}
	return spInstance;

}

void ResourceManager::freeResources()
{
	if (spInstance)
	{
		delete spInstance;
		spInstance = NULL;
	}
}

ResourceManager::~ResourceManager()
{
	if (spInstance)
	{
		delete spInstance;
		spInstance = NULL;
	}
}

Model* ResourceManager::loadModel(int id)
{
	std::map<int, Model*>::iterator it = loadedModels.begin();
	while (it != loadedModels.end())
	{
		if (it->first == id)
		{
			it->second->Load();
			return it->second;
		}
		it++;
	}

	std::map<int, ModelResource*>::iterator res_it = modelResources.begin();
	while (res_it != modelResources.end())
	{
		if (res_it->first == id)
		{
			Model* model = new Model();
			model->mr = res_it->second;
			model->Load();
			loadedModels.insert(std::pair<int, Model*>(id, model));
			return model;
		}
		res_it++;
	}
}

Texture* ResourceManager::loadTexture(int id)
{
	std::map<int, Texture*>::iterator it = loadedTextures.begin();
	while (it != loadedTextures.end())
	{
		if (it->first == id)
		{
			it->second->Load();
			return it->second;
		}
		it++;
	}
	std::map<int, TextureResource*>::iterator res_it = textureResources.begin();
	while (res_it != textureResources.end())
	{
		if (res_it->first == id)
		{
			Texture* texture = new Texture();
			texture->tr = res_it->second;
			texture->Load();
			loadedTextures.insert(std::pair<int, Texture*>(id, texture));
			return texture;
		}
		res_it++;
	}
}

Shader* ResourceManager::loadShader(int id)
{
	std::map<int, Shader*>::iterator it = loadedShaders.begin();
	while (it != loadedShaders.end())
	{
		if (it->first == id)
		{
			it->second->Load();
			return it->second;
		}
		it++;
	}
	std::map<int, ShaderResource*>::iterator res_it = shaderResources.begin();
	while (res_it != shaderResources.end())
	{
		if (res_it->first == id)
		{
			Shader* shader = new Shader();
			shader->sr = res_it->second;
			shader->Load();
			loadedShaders.insert(std::pair<int, Shader*>(id, shader));
			return shader;
		}
		res_it++;
	}
}

Model::Model()
{
	mr = new ModelResource();
}

Model::~Model()
{
	delete mr;
}

void Model::Load()
{
	std::vector<Vertex> vertexVector;
	std::vector<unsigned short> indexVector;

	readNfg(this->mr->file, vertexVector, indexVector);

	this->indexCount = indexVector.size();
	int vertexCount = vertexVector.size();

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(Vertex), vertexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(unsigned short), indexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

Texture::Texture()
{
	tr = new TextureResource();
	tr->type = "normal";
}

Texture::~Texture()
{
	delete tr;
}

void Texture::Load()
{
	int width, height, bpp;
	char* pixelArray;

	pixelArray = LoadTGA((tr->file).c_str(), &width, &height, &bpp);

	GLenum textureType = (tr->type == "cube") ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
	glGenTextures(1, &textureId);
	glBindTexture(textureType, textureId);

	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, tr->wrap_t);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, tr->mag_filter);

	if (textureType == GL_TEXTURE_CUBE_MAP) {
		int bytesPerPixel = bpp / 8;
		int faceWidth = width / 4;
		int faceHeight = height / 3;

		int xOffsets[6] = { 2, 0, 1, 1, 1, 3 };
		int yOffsets[6] = { 1, 1, 0, 2, 1, 1 };

		for (int face = 0; face < 6; face++) {
			int xOffset = xOffsets[face] * faceWidth;
			int yOffset = yOffsets[face] * faceHeight;
			char* subBuffer = new char[faceWidth * faceHeight * bytesPerPixel];
			for (int row = 0; row < faceHeight; row++) {
				memcpy(
					subBuffer + row * faceWidth * bytesPerPixel,
					pixelArray + ((yOffset + row) * width + xOffset) * bytesPerPixel,
					faceWidth * bytesPerPixel
				);
			}
			GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
			GLenum format = (bytesPerPixel == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(
				faceTarget, 0, format,
				faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, subBuffer
			);
			delete[] subBuffer;
		}
	}
	else {
		if (bpp == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);
	}
}

void Shader::SetFogParameters(Vector3& fogColor, float smallRadius, float largeRadius)
{
	glUniform3f(fogColorUniform, fogColor.x, fogColor.y, fogColor.z);
	glUniform1f(smallRadiusUniform, smallRadius);
	glUniform1f(largeRadiusUniform, largeRadius);
}

Shader::Shader()
{
	sr = new ShaderResource();
}

Shader::~Shader()
{
	delete sr;
	glDeleteProgram(program);
}

void Shader::Load()
{
	char* charFileVS = new char;
	char* charFileFS = new char;
	charFileVS = (char*)(sr->fileVS.c_str());
	charFileFS = (char*)(sr->fileFS.c_str());

	vertexShader = esLoadShader(GL_VERTEX_SHADER, charFileVS);

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, charFileFS);

	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	matrixUniform = glGetUniformLocation(program, "u_rotation");
	positionAttribute = glGetAttribLocation(program, "a_posL");
	heightUniform = glGetUniformLocation(program, "u_height");
	colorAttribute = glGetAttribLocation(program, "a_color");
	uv2Attribute = glGetAttribLocation(program, "a_uv2");
	uvAttribute = glGetAttribLocation(program, "a_uv");

	fogColorUniform = glGetUniformLocation(program, "u_fogColor");
	smallRadiusUniform = glGetUniformLocation(program, "u_smallRadius");
	largeRadiusUniform = glGetUniformLocation(program, "u_largeRadius");
	cameraPosUniform = glGetUniformLocation(program, "u_cameraPos");

	modelMatrixUniform = glGetUniformLocation(program, "u_modelMatrix");

	for (int i = 0; i < MAX_TEXTURES; i++) {
		std::string uniformName = "u_texture_" + std::to_string(i);
		textureUniform[i] = glGetUniformLocation(program, uniformName.c_str());
	}

	MVP = glGetUniformLocation(program, "uMVP");
	uvOffsetUniform = glGetUniformLocation(program, "u_uvOffset");


}

void readNfg(std::string nfgPath, std::vector<Vertex>& vertexVector, std::vector<unsigned short>& indexVector)
{
	Vertex aux;

	std::string line;
	std::ifstream file(nfgPath);

	int nrVertices = 0;
	std::getline(file, line);

	nrVertices = std::stoi(&line[12]);
	for (int i = 0; i < nrVertices; i++)
	{
		// pos
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.pos.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.pos.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.pos.z = std::stof(&line[1]);

		// norm
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.norm.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.norm.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.norm.z = std::stof(&line[1]);

		// binorm
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.binorm.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.binorm.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.binorm.z = std::stof(&line[1]);

		// tgt
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.tgt.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.tgt.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.tgt.z = std::stof(&line[1]);

		// uv
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.uv.x = std::stof(&line[0]);

		std::getline(file, line, ']');
		aux.uv.y = std::stof(&line[1]);

		vertexVector.push_back(aux);
	}
	std::getline(file, line);

	std::getline(file, line);
	//indexCount = std::stoi(&line[11]);

	while (std::getline(file, line, ' '))
	{
		if (line.length() != 0 && line[line.length() - 1] != '.')
		{
			indexVector.push_back((unsigned short)std::stoi(line));
		}

	}

	file.close();

}
