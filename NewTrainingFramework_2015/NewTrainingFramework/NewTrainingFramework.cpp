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
#include "ResourceManager.h"
#include "SceneManager.h"


#define PI 3.14159265358979323846

GLuint vboId,vbold,iboId,modelVboId,modelIboId;
GLuint textureId;
Shader myShaders, lineShader, modelShader;
float totalTime = 0.0f;
const void* ptr_iboId;
GLsizei indexCount;
ResourceManager* rm;
SceneManager* sm;

Camera *camera;

int Init ( ESContext *esContext )
{
	rm = ResourceManager::getInstance();
	rm->Init();

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	sm = SceneManager::getInstance();
	sm->Init();
	camera = sm->getActiveCamera();


	glEnable(GL_DEPTH_TEST);

	return 0;
}
void Draw ( ESContext *esContext )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sm->Draw(esContext);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{
	sm->Update(deltaTime);
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed == 1)
	{
		switch (key)
		{
			case 'W': case 'w':
				camera->moveOz(-1);
				break;

			case 'S': case 's':
				camera->moveOz(1);
				break;

			case 'D': case 'd':
				camera->moveOx(1);
				break;
			
			case 'A': case 'a':
				camera->moveOx(-1);
				break;

			case 'Q': case 'q':
				camera->rotateOz(1);
				break;

			case 'E': case 'e':
				camera->rotateOz(-1);
				break;

			case VK_LEFT:
				camera->rotateOy(1);
				break;

			case VK_RIGHT:
				camera->rotateOy(-1);
				break;

			case VK_UP:
				camera->rotateOx(1);
				break;

			case VK_DOWN:
				camera->rotateOx(-1);
				break;

			case VK_SPACE:
				camera->moveOy(1);
				break;

			case VK_CONTROL:
				camera->moveOy(-1);
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
	esRegisterKeyFunc ( &esContext, Key );
	esRegisterMouseFunc ( &esContext, Mouse );

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}


