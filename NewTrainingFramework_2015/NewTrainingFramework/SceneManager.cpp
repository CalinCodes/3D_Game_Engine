#include "stdafx.h"
#include "SceneManager.h"

//float totalTime = 0.0f;
SceneManager* SceneManager::spInstance = NULL;

SceneManager::SceneManager()
{

}

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

	///
	Camera* camera = new Camera();
	cameras.push_back(camera);

	///

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
				so->type = object->first_node("type")->value();
				so->name = object->first_node("name")->value();
				//so->depth_test = std::stoi(object->first_node("depth_test")->value());

				if (so->type == "normal")
				{
					rm->loadModel(std::stoi(object->first_node("model")->value()));
					rm->loadShader(std::stoi(object->first_node("shader")->value()));

					so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];
				}

				so->shader = rm->loadedShaders[std::stoi(object->first_node("shader")->value())];

				int k = 0;
				for (rapidxml::xml_node<>* texture = object->first_node("textures")->first_node("texture"); texture; texture = texture->next_sibling())
				{
					rm->loadTexture(std::stoi(texture->first_attribute("id")->value()));
					so->textures.push_back(rm->loadedTextures[std::stoi(texture->first_attribute("id")->value())]);
					k++;
				}
				

				objects.insert(std::pair<int, SceneObject*>(so->id, so));
			}
		}
	}
}

Camera* SceneManager::getActiveCamera()
{
	return cameras[activeCamera];
}

void SceneManager::Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	std::map<int, SceneObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->Draw(esContext);
		it++;
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

SceneManager* SceneManager::getInstance()
{
	if (!spInstance)
	{
		spInstance = new SceneManager();
	}
	return spInstance;

}

void SceneObject::Draw(ESContext* esContext)
{
	sendCommonData(esContext);
	sendSpecificData(esContext);
}

void SceneObject::sendCommonData(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();
	Matrix MVP = camera->viewMatrix * camera->perspectiveMatrix;
	ResourceManager* rm = ResourceManager::getInstance();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	int i = 0;
	std::vector<Texture*>::iterator it = textures.begin();
	while (it != textures.end())
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->textureId);
		glUniform1i(shader->textureUniform, i);
		i++; it++;
	}

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
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

	if (shader->textureUniform != -1)
	{
		glUniform1i(shader->textureUniform, 0);
	}

	glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneObject::sendSpecificData(ESContext* esContext)
{
}

void SceneObject::Update(float deltaTime)
{

}
