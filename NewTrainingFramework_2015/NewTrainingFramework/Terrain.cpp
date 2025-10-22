#include "stdafx.h"
#include "SceneManager.h"

Terrain::Terrain(SceneObject *so) {
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
	this->generateModel();
}

void Terrain::generateModel() {
	std::vector<Vertex> vertexVector;
	std::vector<unsigned short> indexVector;

	for (unsigned short i = 0; i <= nrCells; i++) {
		for (unsigned short j = 0; j <= nrCells; j++) {
			Vertex vertex;
			vertex.pos = Vector3(i * dimCell - nrCells / 2 * dimCell, offsetY, j * dimCell - nrCells / 2 * dimCell);
			vertex.norm = Vector3(0, 1, 0);
			vertex.uv = Vector2((float)i, (float)j);
			vertex.uv2 = vertex.uv / nrCells;
			vertexVector.push_back(vertex);
		}
	}

	for (unsigned short i = 0; i < nrCells; i++) {
		for (unsigned short j = 0; j < nrCells; j++) {
			indexVector.push_back(i * (nrCells + 1) + j);
			indexVector.push_back(i * (nrCells + 1) + j + 1);
			indexVector.push_back((i + 1) * (nrCells + 1) + j);

			indexVector.push_back((i + 1) * (nrCells + 1) + j);
			indexVector.push_back((i + 1) * (nrCells + 1) + j + 1);
			indexVector.push_back(i * (nrCells + 1) + j + 1);
		}
	}

	model = new Model();
	model->indexCount = indexVector.size();

	glGenBuffers(1, &model->vboId);
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(Vertex), vertexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &model->iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(unsigned short), indexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	SceneManager* sm = SceneManager::getInstance();
	sm->objects.insert(std::pair<int, SceneObject*>(this->id, this));
}

void Terrain::sendSpecificData(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();

	if (shader->heightUniform != -1) {
		glUniform3f(shader->heightUniform, terrainHeights.x, terrainHeights.y, terrainHeights.z);
	}

	if (shader->uvOffsetUniform != -1) {
		glUniform2f(shader->uvOffsetUniform, uvOffset.x, uvOffset.y);
	}

	if (shader->cameraPosUniform != -1) {
		glUniform3f(shader->cameraPosUniform, camera->position.x, camera->position.y, camera->position.z);
	}

}

void Terrain::Update(float deltaTime)
{
	Camera* camera = SceneManager::getInstance()->getActiveCamera();

	float dx = camera->position.x - position.x;
	float dz = camera->position.z - position.z;

	if (fabs(dx) > dimCell) {
		if (dx > 0) {
			position.x += dimCell;
			uvOffset.x += 1.0f / nrCells;
		}
		else {
			position.x -= dimCell;
			uvOffset.x -= 1.0f / nrCells;
		}
	}

	if (fabs(dz) > dimCell) {
		if (dz > 0) {
			position.z += dimCell;
			uvOffset.y += 1.0f / nrCells;
		}
		else {
			position.z -= dimCell;
			uvOffset.y -= 1.0f / nrCells;
		}
	}

}