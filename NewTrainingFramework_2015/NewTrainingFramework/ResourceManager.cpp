#include "stdafx.h"
#include "ResourceManager.h"
#include "../Utilities/TGA.cpp"

void readNfg(std::string nfgPath, std::vector<Vertex>& vertexVector, std::vector<unsigned short>& indexVector);
char* LoadTGA(const char* szFileName, int* width, int* height, int* bpp);

ResourceManager *ResourceManager::spInstance = NULL;

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

	int indexCount = indexVector.size();
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

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	if (bpp == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tr->wrap_t);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tr->mag_filter);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Shader::Shader()
{
	sr = new ShaderResource();
}

Shader::~Shader()
{
	delete sr;
}

void Shader::Load()
{
	Shaders shader;
    shader.Init(const_cast<char*>(sr->fileVS.c_str()), const_cast<char*>(sr->fileFS.c_str()));
	program = shader.program;
}
