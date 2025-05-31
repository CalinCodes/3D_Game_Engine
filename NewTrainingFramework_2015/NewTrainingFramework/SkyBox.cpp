#include "stdafx.h"
#include "SceneManager.h"

void SkyBox::Update(float deltaTime)
{
	Camera* camera = SceneManager::getInstance()->getActiveCamera();
	position.x = camera->position.x;
	position.z = camera->position.z;
}

SkyBox::SkyBox(SceneObject* so) {
	this->id = so->id;
	this->position = so->position;
	this->rotation = so->rotation;
	this->scale = so->scale;
	this->model = so->model;
	this->shader = so->shader;
	this->textures = so->textures;
	this->type = so->type;
	this->name = so->name;
	this->depth_test = so->depth_test;
}