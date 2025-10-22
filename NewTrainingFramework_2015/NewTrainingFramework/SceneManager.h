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

   virtual float getKspec() const { return 1.0f; }
   virtual float getKdiff() const { return 1.0f; }

   virtual void Draw(ESContext *esContext);  
   virtual void sendCommonData(ESContext* esContext);  
   virtual void sendSpecificData(ESContext *esContext);  
   virtual void Update(float deltaTime);  
};

class Light {
public:
	int id;
	int type; // POINT, DIRECTIONAL, SPOT
	Vector3 position;
	Vector3 direction;
	Vector3 colorDiffuse;
	Vector3 colorSpecular;
	float specPower;
	float spotCutoff;
	float spotExponent;
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
	std::vector<Light*> lights;

	Vector3 ambientColor = Vector3(0.2f, 0.2f, 0.2f);
	float ambientRatio = 0.5f;
	
	Camera* getActiveCamera();
	int activeCamera = 0;
	float totalTime;
	Vector3 fogColor;
	float smallRadius, largeRadius;
};

class Terrain : public SceneObject {
public:
	const int nrCells = 100, dimCell = 10, offsetY = -100;
	virtual float getKspec() const override { return 0.1f; }
	virtual float getKdiff() const override { return 1.0f; }
	Vector3 terrainHeights;
	Vector2 uvOffset;
	void generateModel();
	void sendSpecificData(ESContext* esContext) override;
	void Update(float deltaTime) override;
	Terrain(SceneObject* so);
};

class SkyBox : public SceneObject {
public:
	void Update(float deltaTime) override;
	SkyBox(SceneObject* so);
};

class Fire : public SceneObject {
public:
	float u_DispMax;
	float u_Time;
	clock_t lastClock;
	float speedFactor;
	Fire(SceneObject* so);
	void Update(float deltaTime) override;
	void sendSpecificData(ESContext* esContext) override;
};

class ReflectedObject : public SceneObject {
public:
	float reflectedAmmount = 0.5;
	ReflectedObject(SceneObject* so);
	void sendSpecificData(ESContext* esContext) override;
	void Update(float deltaTime) override;
};
