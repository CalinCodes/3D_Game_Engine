#include "stdafx.h"
#include "ResourceManager.h"
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include "Vertex.h"
#include "../Utilities/TGA.cpp"

void readNfg(std::string nfgPath, std::vector<Vertex>& vertexVector, std::vector<unsigned short>& indexVector);
char* LoadTGA(const char* szFileName, int* width, int* height, int* bpp);

ResourceManager *ResourceManager::spInstance = NULL;

ResourceManager::ResourceManager()
{

}

void ResourceManager::Init()
{

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tr->min_filter);

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
	int indexCount = std::stoi(&line[11]);

	while (std::getline(file, line, ' '))
	{
		if (line.length() != 0 && line[line.length() - 1] != '.')
		{
			indexVector.push_back((unsigned short)std::stoi(line));
		}

	}

	file.close();

}

char* LoadTGA(const char* szFileName, int* width, int* height, int* bpp)
{

	FILE* f;

	if (fopen_s(&f, szFileName, "rb") != 0)
		return NULL;

	TGA_HEADER header;
	fread(&header, sizeof(header), 1, f);

	fseek(f, 0, SEEK_END);
	int fileLen = ftell(f);
	fseek(f, sizeof(header) + header.identsize, SEEK_SET);

	if (header.imagetype != IT_COMPRESSED && header.imagetype != IT_UNCOMPRESSED)
	{
		fclose(f);
		return NULL;
	}

	if (header.bits != 24 && header.bits != 32)
	{
		fclose(f);
		return NULL;
	}

	int bufferSize = fileLen - sizeof(header) - header.identsize;
	char* pBuffer = new char[bufferSize];
	fread(pBuffer, 1, bufferSize, f);
	fclose(f);

	*width = header.width;
	*height = header.height;
	*bpp = header.bits;
	char* pOutBuffer = new char[header.width * header.height * header.bits / 8];

	switch (header.imagetype)
	{
	case IT_UNCOMPRESSED:
		LoadUncompressedImage(pOutBuffer, pBuffer, &header);
		break;
	case IT_COMPRESSED:
		LoadCompressedImage(pOutBuffer, pBuffer, &header);
		break;
	}

	delete[] pBuffer;

	return pOutBuffer;
}