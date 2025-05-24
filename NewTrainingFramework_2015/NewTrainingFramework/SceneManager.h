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

	virtual void Draw(ESContext *esContext);
	virtual void sendCommonData(ESContext* esContext);
	void sendSpecificData(ESContext *esContext);
	virtual void Update(float deltaTime);

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
	float totalTime;
	Vector3 fogColor;
	float smallRadius, largeRadius;
};

class Terrain : public SceneObject {
public:
	const int nrCells = 100, dimCell = 10, offsetY = -100;
	Vector3 terrainHeights;
	Vector2 uvOffset;
	void generateModel();
	void sendCommonData(ESContext* esContext) override;
	void Update(float deltaTime) override;
	Terrain(SceneObject* so);
};

class SkyBox : public SceneObject {
public:
	void sendCommonData(ESContext* esContext) override;
	void Update(float deltaTime) override;
	SkyBox(SceneObject* so);
};

class Fire : public SceneObject {
public:
	float u_DispMax;
	float u_Time;
	clock_t lastClock;
	float speedFactor;
	Fire();
	void Update(float deltaTime) override;
	void Draw(ESContext* esContext) override;
};
