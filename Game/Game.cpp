// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
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
	// Set the view parameters in renderer
	Graphics::IRenderer::mRenderer->SetFOV(60.0F);
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

	glDisable(GL_LIGHTING);

	glRotatef(rotate, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, -1000);
	glVertex3f(100, 50, -1000);
	glVertex3f(640/2, 0, -1000);
	glEnd();

	glutSwapBuffers();
}

