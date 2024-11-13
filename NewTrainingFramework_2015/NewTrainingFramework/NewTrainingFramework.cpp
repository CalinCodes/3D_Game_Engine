// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"

#define PI 3.14159265358979323846

GLuint vboId,Vbold;
Shaders myShaders, lineShader;
float angle = 0.0f, step = 0.01f;
float totalTime = 0.0f;

Camera camera;
Matrix MVP;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[6];
	Vertex lineVerticesData[2];

	verticesData[0].pos.x =  -0.5f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = 0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;

	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = -0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[4].pos.x = 0.5f;  verticesData[4].pos.y = 0.5f;  verticesData[4].pos.z = 0.0f;
	verticesData[5].pos.x = -0.5f;  verticesData[5].pos.y = -0.5f;  verticesData[5].pos.z = 0.0f;

	verticesData[3].color.x = 1.0f; verticesData[3].color.y = 1.0f; verticesData[3].color.z = 0.0f;
	verticesData[4].color.x = 0.0f; verticesData[4].color.y = 0.0f; verticesData[4].color.z = 1.0f;
	verticesData[5].color.x = 0.0f; verticesData[5].color.y = 1.0f; verticesData[5].color.z = 0.0f;

	lineVerticesData[0].pos.x = 0.0f;  lineVerticesData[0].pos.y = 1.0f;  lineVerticesData[0].pos.z = 0.0f;
	lineVerticesData[1].pos.x = 0.0f; lineVerticesData[1].pos.y = -1.0f;  lineVerticesData[1].pos.z = 0.0f;
	


	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &Vbold);
	glBindBuffer(GL_ARRAY_BUFFER, Vbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//creation of shaders and program
	lineShader.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");

}

void Draw ( ESContext *esContext )
{
	Matrix mRotation;
	mRotation.SetRotationZ(angle);

	MVP = camera.viewMatrix * camera.perspectiveMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	// Drawing triangles
	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if(myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if(myShaders.matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)mRotation.m);
	}

	if (myShaders.MVP != -1)
	{
		glUniformMatrix4fv(myShaders.MVP, 1, GL_FALSE, (float*)MVP.m);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Drawing lines
	glUseProgram(lineShader.program);
	glBindBuffer(GL_ARRAY_BUFFER, Vbold);

	if (lineShader.positionAttribute != -1)
	{
		glEnableVertexAttribArray(lineShader.positionAttribute);
		glVertexAttribPointer(lineShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	glDrawArrays(GL_LINES, 0, 2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	angle += step;
	if (angle > 2*PI) angle -= 2*PI;
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed == 1)
	{
		//printf("%c", key);
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
				if (x <= Globals::screenWidth/2)
				{
					//printf("LEFT SIDE\n");
					camera.rotateOz(-1);
				}
				else
				{
					//printf("RIGHT SIDE\n");
					camera.rotateOz(1);
				}
				break;
			case RIGHT_CLICK:
				
				break;
		}

		
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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

