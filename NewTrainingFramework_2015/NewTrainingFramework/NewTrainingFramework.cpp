// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>


#define PI 3.14159265358979323846

GLuint vboId,vbold,iboId,modelVboId,modelIboId;
GLuint textureId;
Shaders myShaders, lineShader, modelShader;
float angle = 0.0f, step = 0.01f;
float totalTime = 0.0f;
const void* ptr_iboId;
GLsizei indexCount;

Camera camera;
Matrix MVP;

void readNfg(std::string nfgPath, std::vector<Vertex> &vertexVector, std::vector<unsigned short> &indexVector);

int Init ( ESContext *esContext )
{
	int width, height, bpp;
	char* pixelArray;

	pixelArray = LoadTGA("../../NewResourcesPacket/Textures/Croco.tga", &width, &height, &bpp);

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[4];
	Vertex lineVerticesData[2];
	unsigned short verticesIndex[6];

	std::vector<Vertex> vertexVector;
	std::vector<unsigned short> indexVector;

	readNfg("../../NewResourcesPacket/Models/Croco.nfg",vertexVector,indexVector);

	/*verticesData[0].pos.x =  -0.5f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;
	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = 0.5f;  verticesData[3].pos.z = 0.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;
	verticesData[3].color.x = 1.0f; verticesData[3].color.y = 1.0f; verticesData[3].color.z = 0.0f;

	verticesIndex[0] = 0;
	verticesIndex[1] = 1;
	verticesIndex[2] = 2;
	verticesIndex[3] = 0;
	verticesIndex[4] = 2;
	verticesIndex[5] = 3;


	lineVerticesData[0].pos.x = 0.0f;  lineVerticesData[0].pos.y = 1.0f;  lineVerticesData[0].pos.z = 0.0f;
	lineVerticesData[1].pos.x = 0.0f; lineVerticesData[1].pos.y = -1.0f;  lineVerticesData[1].pos.z = 0.0f;*/
	


	//buffer object
	glGenBuffers(1, &modelVboId);
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
    glBufferData(GL_ARRAY_BUFFER, vertexVector.size() * sizeof(Vertex), vertexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &modelIboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexVector.size() * sizeof(unsigned short), indexVector.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	if (bpp == 32)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelArray);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelArray);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);


	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbold);
	glBindBuffer(GL_ARRAY_BUFFER, vbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);

	//creation of shaders and program
	modelShader.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	lineShader.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	Matrix mRotation;
	mRotation.SetRotationZ(angle);

	MVP = camera.viewMatrix * camera.perspectiveMatrix;

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Drawing triangles
	//glUseProgram(myShaders.program);

	//glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	//
	//if(myShaders.positionAttribute != -1)
	//{
	//	glEnableVertexAttribArray(myShaders.positionAttribute);
	//	glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//}

	//if(myShaders.colorAttribute != -1)
	//{
	//	glEnableVertexAttribArray(myShaders.colorAttribute);
	//	glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	//}

	//if(myShaders.matrixUniform != -1)
	//{
	//	glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)mRotation.m);
	//}

	//if (myShaders.MVP != -1)
	//{
	//	glUniformMatrix4fv(myShaders.MVP, 1, GL_FALSE, (float*)MVP.m);
	//}

	//// glDrawArrays(GL_TRIANGLES, 0, 4);
	//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT, ptr_iboId);

	//// Drawing lines
	//glUseProgram(lineShader.program);
	//glBindBuffer(GL_ARRAY_BUFFER, vbold);

	//if (lineShader.positionAttribute != -1)
	//{
	//	glEnableVertexAttribArray(lineShader.positionAttribute);
	//	glVertexAttribPointer(lineShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//}

	//glDrawArrays(GL_LINES, 0, 2);
	glUseProgram(modelShader.program);

	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);

	glBindTexture(GL_TEXTURE_2D, textureId);

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
		glBindTexture(GL_TEXTURE_2D, textureId);
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

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;
	if (totalTime >= Globals::frameTime)
	{
		totalTime = totalTime - Globals::frameTime;

		camera.deltaTime = deltaTime;
		camera.updateWorldView();
	}

}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed == 1)
	{
		switch (key)
		{
			case 'W': case 'w':
				camera.moveOz(-1);
				break;

			case 'S': case 's':
				camera.moveOz(1);
				break;

			case 'D': case 'd':
				camera.moveOx(1);
				break;
			
			case 'A': case 'a':
				camera.moveOx(-1);
				break;

			case 'Q': case 'q':
				camera.rotateOz(1);
				break;

			case 'E': case 'e':
				camera.rotateOz(-1);
				break;

			case VK_LEFT:
				camera.rotateOy(1);
				break;

			case VK_RIGHT:
				camera.rotateOy(-1);
				break;

			case VK_UP:
				camera.rotateOx(1);
				break;

			case VK_DOWN:
				camera.rotateOx(-1);
				break;

			case VK_SPACE:
				camera.moveOy(1);
				break;

			case VK_CONTROL:
				camera.moveOy(-1);
				break;
		}
	}
}

void Mouse(ESContext* esContext, MouseButtons btn, MouseEvents event, int x, int y)
{
	if (event == BTN_DOWN)
	{
		switch (btn)
		{
			case LEFT_CLICK:
				
				break;
			case RIGHT_CLICK:
				
				break;
		}

		
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &vbold);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "3D Game Engine", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc ( &esContext, Mouse );

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

void readNfg(std::string nfgPath, std::vector<Vertex> &vertexVector, std::vector<unsigned short> &indexVector)
{
	Vertex aux;

	std::string line;
	std::ifstream file(nfgPath);

	int nrVertices = 0;
	std::getline(file, line);
	
	nrVertices = std::stoi(&line[12]);
	for (int i = 0; i < nrVertices; i++)
	{
		// pos
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.pos.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.pos.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.pos.z = std::stof(&line[1]);

		// norm
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.norm.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.norm.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.norm.z = std::stof(&line[1]);

		// binorm
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.binorm.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.binorm.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.binorm.z = std::stof(&line[1]);

		// tgt
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.tgt.x = std::stof(&line[0]);

		std::getline(file, line, ',');
		aux.tgt.y = std::stof(&line[1]);

		std::getline(file, line, ']');
		aux.tgt.z = std::stof(&line[1]);

		// uv
		std::getline(file, line, '[');

		std::getline(file, line, ',');
		aux.uv.x = std::stof(&line[0]);

		std::getline(file, line, ']');
		aux.uv.y = std::stof(&line[1]);

		vertexVector.push_back(aux);
	}
	std::getline(file, line);

	std::getline(file, line);
	indexCount = std::stoi(&line[11]);

	while (std::getline(file, line, ' '))
	{
		if (line.length() != 0 && line[line.length() - 1] != '.')
		{
			indexVector.push_back((unsigned short)std::stoi(line));
		}

	}

	file.close();
	
}
