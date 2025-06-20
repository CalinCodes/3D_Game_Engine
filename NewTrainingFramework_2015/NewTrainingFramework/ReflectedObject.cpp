#include "stdafx.h"
#include "SceneManager.h"

ReflectedObject::ReflectedObject(SceneObject* so) {
    this->id = so->id;
    this->position = so->position;
    this->rotation = so->rotation;
    this->scale = so->scale;
    this->model = so->model;
    this->shader = so->shader;
    this->textures = so->textures;
    this->name = so->name;
    this->depth_test = so->depth_test;
}

void ReflectedObject::sendSpecificData(ESContext* esContext) {
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();

    if (shader->normalAttribute != -1) {
        glEnableVertexAttribArray(shader->normalAttribute);
        glVertexAttribPointer(shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
    }

    if (shader->cameraPosUniform != -1) {
        glUniform3f(shader->cameraPosUniform, camera->position.x, camera->position.y, camera->position.z);
    }

	if (shader->reflectionUniform != -1) {
		glUniform1f(shader->reflectionUniform, this->reflectedAmmount);
	}
}

void ReflectedObject::Update(float deltaTime)
{
}
