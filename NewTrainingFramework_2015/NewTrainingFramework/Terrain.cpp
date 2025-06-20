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


void Terrain::sendCommonData(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();

	Matrix translationMatrix, modelMatrix, xRotationMatrix, yRotationMatrix, zRotationMatrix;
	translationMatrix.SetTranslation(position);
	xRotationMatrix.SetRotationX(rotation.x);
	yRotationMatrix.SetRotationY(rotation.y);
	zRotationMatrix.SetRotationZ(rotation.z);
	modelMatrix = xRotationMatrix * yRotationMatrix * zRotationMatrix * translationMatrix;

	Matrix MVP = modelMatrix * camera->viewMatrix * camera->perspectiveMatrix;
	ResourceManager* rm = ResourceManager::getInstance();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	for (int i = 0; i < textures.size() && i < MAX_TEXTURES; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->textureId);
		glUniform1i(shader->textureUniform[i], i);
	}

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shader->uv2Attribute != -1)
	{
		glEnableVertexAttribArray(shader->uv2Attribute);
		glVertexAttribPointer(shader->uv2Attribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv2));
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (shader->MVP != -1)
	{
		glUniformMatrix4fv(shader->MVP, 1, GL_FALSE, (float*)MVP.m);
	}

	if(shader->heightUniform != -1) {
		glUniform3f(shader->heightUniform, terrainHeights.x, terrainHeights.y, terrainHeights.z);
	}

	if (shader->uvOffsetUniform != -1) {
		glUniform2f(shader->uvOffsetUniform, uvOffset.x, uvOffset.y);
	}

	if (shader->cameraPosUniform != -1) {
		glUniform3f(shader->cameraPosUniform, camera->position.x, camera->position.y, camera->position.z);
	}

	if (shader->modelMatrixUniform != -1)
	{
		glUniformMatrix4fv(shader->modelMatrixUniform, 1, GL_FALSE, (float*)modelMatrix.m);
	}

	if (shader->normalAttribute != -1) {
		glEnableVertexAttribArray(shader->normalAttribute);
		glVertexAttribPointer(shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	}


	glUniform3f(glGetUniformLocation(shader->program, "u_ambientColor"), sm->ambientColor.x, sm->ambientColor.y, sm->ambientColor.z);
	glUniform1f(glGetUniformLocation(shader->program, "u_ambientRatio"), sm->ambientRatio);

	glUniform1i(glGetUniformLocation(shader->program, "u_numLights"), sm->lights.size());
	for (int i = 0; i < sm->lights.size(); ++i) {
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightPositions[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->position.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightDirections[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->direction.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightDiffuse[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->colorDiffuse.x);
		glUniform3fv(glGetUniformLocation(shader->program, ("u_lightSpecular[" + std::to_string(i) + "]").c_str()), 1, &(sm->lights[i])->colorSpecular.x);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpecPower[" + std::to_string(i) + "]").c_str()), sm->lights[i]->specPower);
		glUniform1i(glGetUniformLocation(shader->program, ("u_lightTypes[" + std::to_string(i) + "]").c_str()), sm->lights[i]->type);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpotCutoff[" + std::to_string(i) + "]").c_str()), sm->lights[i]->spotCutoff);
		glUniform1f(glGetUniformLocation(shader->program, ("u_lightSpotExponent[" + std::to_string(i) + "]").c_str()), sm->lights[i]->spotExponent);
	}
	glUniform1f(glGetUniformLocation(shader->program, "u_kspec"), 0.1);
	glUniform1f(glGetUniformLocation(shader->program, "u_kdiff"), 1);
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