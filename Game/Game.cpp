// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CShaderHolder.h"
#include "GL/glut.h"


bool 
EngineCore::IGame::Create()
{
	IGame::mGame = new Game();
	return (IGame::mGame != nullptr);
}

Game::Game() : EngineCore::IGame()
{
}

Game::~Game()
{
}

bool Game::PostRendererInitialize()
{
	//TODO: initialize lighting default values (some basic light somewhere)
	CShaderHolder::Create();

	// Set the view parameters in renderer
	Graphics::IRenderer::mRenderer->SetFOV(60.0F);
	// create model holder
	CModelHolder::s_pInstance->Create("..\\Game\\Assets\\model.zip");

	// load models
	CModelHolder::s_pInstance->LoadModel("skull.obj");

	return true;
}

void Game::UpdateObjects(float dt)
{

}

void Game::Render()
{
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// adjust camera projection and view according to the current 
	// frustum parameters (3d - perspective mode)
	Graphics::IRenderer::mRenderer->PrepareCamera3D();
	glColor4f(1.0, 0.0, 1.0, 0.5);
	static float rotate = 0.0f;
	rotate += 0.01f;

	glEnable(GL_LIGHTING);

	// enable vertices array pointer rendering	
	static SModelData m_data;
	if (CModelHolder::s_pInstance->getModelById("skull.obj", m_data))
	{
		glPushMatrix();
		Int32 err = glGetError();
		if (err != 0)
		{
			printf("preparing matrix error = %d\n", err);
		}
		glTranslatef(0, 0, -20);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindVertexArray(m_data.m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, m_data.m_numberOfIndexes, GL_UNSIGNED_SHORT, (void*)(0));
		err = glGetError();
		if (err != 0)
		{
			printf("glError Drawing Model = %d\n", err);
		}

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		err = glGetError();
		if (err != 0)
		{
			printf("glError Drawing Model =%d\n", err);
		}
		glPopMatrix();
	}

	glDisable(GL_LIGHTING);


	glRotatef(rotate, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, -1000);
	glVertex3f(100, 50, -1000);
	glVertex3f(640 / 2, 0, -1000);
	glEnd();

	glutSwapBuffers();
}

