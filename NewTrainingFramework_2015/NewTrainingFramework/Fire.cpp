#include "stdafx.h"
#include "SceneManager.h"
#include <ctime>


Fire::Fire(SceneObject* so) {
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
	this->u_Time = 0.0f;
	this->u_DispMax = 0.1f;
	this->speedFactor = 1.0f;
	this->lastClock = clock();
}

void Fire::Update(float deltaTime) {
    clock_t now = clock();
    float elapsed = float(now - lastClock) / CLOCKS_PER_SEC;
    lastClock = now;
    u_Time += elapsed * speedFactor;
    u_Time = u_Time - floor(u_Time);
}

void Fire::sendSpecificData(ESContext* esContext)
{
    GLint timeLoc = glGetUniformLocation(shader->program, "u_Time");
    GLint dispMaxLoc = glGetUniformLocation(shader->program, "u_DispMax");
    if (timeLoc != -1)
    {
        glUniform1f(timeLoc, u_Time);
    }
    if (dispMaxLoc != -1)
    {
        glUniform1f(dispMaxLoc, u_DispMax);
    }
}