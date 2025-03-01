#include "stdafx.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	ResourceManager* rm = ResourceManager::getInstance();
	rapidxml::file<> file("../../XML/sceneManager.xml");
	char* buffer = new char[file.size() + 1];
	std::memcpy(buffer, file.data(), file.size());
	buffer[file.size()] = '\0';

	rapidxml::xml_document<> doc;
	doc.parse<0>(buffer);
	rapidxml::xml_node<>* root = doc.first_node("sceneManager");
	for (rapidxml::xml_node<>* node = root->first_node(); node; node = node->next_sibling())
	{
		if (strcmp(node->name(), "cameras") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("camera"); folder; folder = folder->next_sibling())
			{
				/////////////////////////////////////////// de parsat pentru toate camerele
			}
		}
		if (strcmp(node->name(), "objects") == 0)
		{
			for (rapidxml::xml_node<>* object = node->first_node("object"); object; object = object->next_sibling())
			{
				SceneObject* so = new SceneObject();
				so->id = std::stoi(object->first_attribute("id")->value());
				so->position.x = std::stof(object->first_node("position")->first_node("x")->value());
				so->position.y = std::stof(object->first_node("position")->first_node("y")->value());
				so->position.z = std::stof(object->first_node("position")->first_node("z")->value());
				so->rotation.x = std::stof(object->first_node("rotation")->first_node("x")->value());
				so->rotation.y = std::stof(object->first_node("rotation")->first_node("y")->value());
				so->rotation.z = std::stof(object->first_node("rotation")->first_node("z")->value());
				so->scale.x = std::stof(object->first_node("scale")->first_node("x")->value());
				so->scale.y = std::stof(object->first_node("scale")->first_node("y")->value());
				so->scale.z = std::stof(object->first_node("scale")->first_node("z")->value());
				so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];
				so->shader = rm->loadedShaders[std::stoi(object->first_node("shader")->value())];
				so->type = object->first_node("type")->value();
				so->name = object->first_node("name")->value();
				so->depth_test = std::stoi(object->first_node("depth_test")->value());

				int k = 0;
				for (rapidxml::xml_node<>* texture = object->first_node("textures")->first_node("texture"); texture; texture = texture->next_sibling())
				{
					so->textures[k] = rm->loadedTextures[std::stoi(texture->first_attribute("id")->value())];
					k++;
				}

				objects.push_back(so);
			}
		}
		delete[] buffer;
	}
}

void SceneManager::sendCommonData()
{
	Camera* camera = getActiveCamera();
	Matrix MVP = camera->viewMatrix * camera->perspectiveMatrix;
	ResourceManager* rm = ResourceManager::getInstance();

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(modelShader.program);

	//glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	glBindBuffer(GL_ARRAY_BUFFER, rm->loadedModels[1]->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rm->loadedModels[1]->iboId);

	//glBindTexture(GL_TEXTURE_2D, textureId);
	glBindTexture(GL_TEXTURE_2D, rm->loadedTextures[4]->textureId);

	if (modelShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.positionAttribute);
		glVertexAttribPointer(modelShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (modelShader.uvAttribute != -1)
	{
		glEnableVertexAttribArray(modelShader.uvAttribute);
		glVertexAttribPointer(modelShader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	}

	if (modelShader.textureUniform != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rm->loadedTextures[4]->textureId);
		glUniform1i(modelShader.textureUniform, 0);
	}

	if (modelShader.MVP != -1)
	{
		glUniformMatrix4fv(modelShader.MVP, 1, GL_FALSE, (float*)MVP.m);
	}

	if (modelShader.textureUniform != -1)
	{
		glUniform1i(modelShader.textureUniform, 0);
	}

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

Camera* SceneManager::getActiveCamera()
{
	return cameras[activeCamera];
}

void SceneManager::Draw()
{
	for (int i = 0; i < objects.size(); i++)
	{
		sendCommonData();
	}
}
