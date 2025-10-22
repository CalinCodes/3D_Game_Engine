#include "stdafx.h"
#include "SceneManager.h"

SceneManager* SceneManager::spInstance = NULL;

SceneManager::SceneManager()
{

}

void SceneManager::Init()
{
	totalTime = 0.0f;

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
		if (strcmp(node->name(), "fog") == 0)
		{
			rapidxml::xml_node<>* colorNode = node->first_node("color");
			fogColor.x = std::stof(colorNode->first_node("r")->value());
			fogColor.y = std::stof(colorNode->first_node("g")->value());
			fogColor.z = std::stof(colorNode->first_node("b")->value());

			smallRadius = std::stof(node->first_node("smallRadius")->value());
			largeRadius = std::stof(node->first_node("largeRadius")->value());
		}
		if (strcmp(node->name(), "cameras") == 0)
		{
			for (rapidxml::xml_node<>* folder = node->first_node("camera"); folder; folder = folder->next_sibling())
			{
				/////////////////////////////////////////// de parsat pentru toate camerele
			}
		}
		if (strcmp(node->name(), "lights") == 0) {
			for (rapidxml::xml_node<>* lightNode = node->first_node("light"); lightNode; lightNode = lightNode->next_sibling()) {
				Light* light = new Light();
				light->id = std::stoi(lightNode->first_attribute("id")->value());
				std::string type = lightNode->first_attribute("type")->value();

				if (type == "point") {
					light->type = 0;
					light->position.x = std::stof(lightNode->first_node("position")->first_node("x")->value());
					light->position.y = std::stof(lightNode->first_node("position")->first_node("y")->value());
					light->position.z = std::stof(lightNode->first_node("position")->first_node("z")->value());
				}
				else if (type == "directional") {
					light->type = 1;
					light->direction.x = std::stof(lightNode->first_node("direction")->first_node("x")->value());
					light->direction.y = std::stof(lightNode->first_node("direction")->first_node("y")->value());
					light->direction.z = std::stof(lightNode->first_node("direction")->first_node("z")->value());
				}
				else if (type == "spot") {
					light->type = 2;
					light->position.x = std::stof(lightNode->first_node("position")->first_node("x")->value());
					light->position.y = std::stof(lightNode->first_node("position")->first_node("y")->value());
					light->position.z = std::stof(lightNode->first_node("position")->first_node("z")->value());
					light->direction.x = std::stof(lightNode->first_node("direction")->first_node("x")->value());
					light->direction.y = std::stof(lightNode->first_node("direction")->first_node("y")->value());
					light->direction.z = std::stof(lightNode->first_node("direction")->first_node("z")->value());
					light->spotCutoff = std::stof(lightNode->first_node("spotCutoff")->value());
					light->spotExponent = std::stof(lightNode->first_node("spotExponent")->value());
				}

				light->colorDiffuse.x = std::stof(lightNode->first_node("diffuse")->first_node("r")->value());
				light->colorDiffuse.y = std::stof(lightNode->first_node("diffuse")->first_node("g")->value());
				light->colorDiffuse.z = std::stof(lightNode->first_node("diffuse")->first_node("b")->value());

				light->colorSpecular.x = std::stof(lightNode->first_node("specular")->first_node("r")->value());
				light->colorSpecular.y = std::stof(lightNode->first_node("specular")->first_node("g")->value());
				light->colorSpecular.z = std::stof(lightNode->first_node("specular")->first_node("b")->value());

				light->specPower = std::stof(lightNode->first_node("specPower")->value());

				lights.push_back(light);
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
				// so->depth_test = std::stoi(object->first_node("depth_test")->value());

				rm->loadShader(std::stoi(object->first_node("shader")->value()));
				so->shader = rm->loadedShaders[std::stoi(object->first_node("shader")->value())];

				int k = 0;
				for (rapidxml::xml_node<>* texture = object->first_node("textures")->first_node("texture"); texture; texture = texture->next_sibling())
				{
					rm->loadTexture(std::stoi(texture->first_attribute("id")->value()));
					so->textures.push_back(rm->loadedTextures[std::stoi(texture->first_attribute("id")->value())]);
					k++;
				}
				
				if (so->type == "normal")
				{
					rm->loadModel(std::stoi(object->first_node("model")->value()));

					so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];

					objects.insert(std::pair<int, SceneObject*>(so->id, so));
				}
				else if (so->type == "terrain")
				{
					Terrain* terrain = new Terrain(so);

					rapidxml::xml_node<>* heightsNode = object->first_node("heights");
					float heightR = std::stof(heightsNode->first_node("r")->value());
					float heightG = std::stof(heightsNode->first_node("g")->value());
					float heightB = std::stof(heightsNode->first_node("b")->value());
					terrain->terrainHeights = Vector3(heightR, heightG, heightB);

					objects.insert(std::pair<int, SceneObject*>(terrain->id, terrain));
				}
				else if (so->type == "skybox")
				{
					rm->loadModel(std::stoi(object->first_node("model")->value()));
					so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];
					
					SkyBox* skybox = new SkyBox(so);
					objects.insert(std::pair<int, SceneObject*>(skybox->id, skybox));
				}
				else if (so->type == "fire")
				{
					rm->loadModel(std::stoi(object->first_node("model")->value()));
					so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];
					Fire* fire = new Fire(so);
					objects.insert(std::pair<int, SceneObject*>(fire->id, fire));
				}
				else if (so->type == "reflected")
				{
					rm->loadModel(std::stoi(object->first_node("model")->value()));
					so->model = rm->loadedModels[std::stoi(object->first_node("model")->value())];
					ReflectedObject* reflected = new ReflectedObject(so);
					
					float reflectedAmmount = std::stof(object->first_node("reflection")->value());
					reflected->reflectedAmmount = reflectedAmmount;

					objects.insert(std::pair<int, SceneObject*>(reflected->id, reflected));
				}
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
	std::map<int, SceneObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->Draw(esContext);
		it++;
	}
}

void SceneManager::Update(float deltaTime)
{
	Camera* camera = getActiveCamera();
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime)
	{
		totalTime = totalTime - Globals::frameTime;

		camera->deltaTime = deltaTime;
		camera->updateWorldView();
	}
	std::map<int, SceneObject*>::iterator it = objects.begin();
	while (it != objects.end())
	{
		it->second->Update(deltaTime);
		it++;
	}
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
	SceneManager* sm = SceneManager::getInstance();

	sendCommonData(esContext);
	shader->SetFogParameters(sm->fogColor, sm->smallRadius, sm->largeRadius);
	sendSpecificData(esContext);


	int err = glGetError();
	glDrawElements(GL_TRIANGLES, model->indexCount, GL_UNSIGNED_SHORT, 0);
	err = glGetError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneObject::sendCommonData(ESContext* esContext)
{
	SceneManager* sm = SceneManager::getInstance();
	Camera* camera = sm->getActiveCamera();

	Matrix scaleMatrix, translationMatrix, modelMatrix, xRotationMatrix, yRotationMatrix, zRotationMatrix;
	scaleMatrix.SetScale(scale.x, scale.y, scale.z);
	translationMatrix.SetTranslation(position);
	xRotationMatrix.SetRotationX(rotation.x);
	yRotationMatrix.SetRotationY(rotation.y);
	zRotationMatrix.SetRotationZ(rotation.z);
	modelMatrix = scaleMatrix * xRotationMatrix * yRotationMatrix * zRotationMatrix * translationMatrix;

	Matrix MVP = modelMatrix * camera->viewMatrix * camera->perspectiveMatrix;
	ResourceManager* rm = ResourceManager::getInstance();

	glUseProgram(shader->program);

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	for(int i = 0; i < textures.size() && i < MAX_TEXTURES; i++)
	{
		if (textures[i]->tr->type == "cube") {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textures[i]->textureId);
			glUniform1i(shader->textureUniform[i], i);
			continue;
		}

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
	glUniform1f(glGetUniformLocation(shader->program, "u_kspec"), this->getKspec());
	glUniform1f(glGetUniformLocation(shader->program, "u_kdiff"), this->getKdiff());
}

void SceneObject::sendSpecificData(ESContext* esContext)  
{  
    
}

void SceneObject::Update(float deltaTime)
{
	return;
}
