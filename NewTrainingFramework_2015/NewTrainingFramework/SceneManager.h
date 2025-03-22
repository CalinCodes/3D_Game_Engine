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
#include "ResourceManager.h"
#include "Camera.h"
#include "Globals.h"


class SceneObject {
public:
	int id;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Model *model;
	Shader *shader;
	std::vector<Texture*> textures;
	std::string type;
	std::string name;
	int depth_test;

	void Draw(ESContext *esContext);
	void sendCommonData(ESContext* esContext);
	void sendSpecificData(ESContext *esContext);
	void Update(float deltaTime);

};

class SceneManager
{
private:
	static SceneManager* spInstance;
	SceneManager();
public:
	void Init();
	static SceneManager* getInstance();
	void InitWindow();
	void Draw(ESContext *esContext);
	void Update(float deltaTime);

	std::map<int, SceneObject*> objects;
	std::vector<Camera*> cameras;
	
	Camera* getActiveCamera();
	int activeCamera = 0;
};


