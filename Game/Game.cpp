// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CShaderHolder.h"
#include "CTextureHolder.h"
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
	CTextureHolder::s_pInstance->Create("..\\Game\\Assets\\textures.zip");

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

	glPushMatrix();
	glEnable(GL_LIGHTING);

	// enable vertices array pointer rendering	
	static SModelData m_data;
	if (CModelHolder::s_pInstance->getModelById("skull.obj", m_data) 
		&& (CShaderHolder::s_pInstance->UseShaderById("model"))
		&& (CTextureHolder::s_pInstance->getTextureById("furTex.bmp")))
	{

		// light set-up
		glEnable(GL_LIGHT0);

		GLfloat lightColor0[] = { 0.8f, 0.8f, 0.0f, 1.0f }; //Color (0.5, 0.5, 0.5)
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
		GLfloat lightPos[] = { 0, 0.5, 0.8, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		Int32 err = glGetError();
		if (err != 0)
		{
			printf("preparing matrix error = %d\n", err);
		}
		//glTranslatef(0, 0, -20);
		static float foolme = 0.0F;
		static bool direction = false;
		CShaderHolder::s_pInstance->GetShaderProgramById("model")->setUniform3f("translate", 0, 0, -20);
		CShaderHolder::s_pInstance->GetShaderProgramById("model")->setUniform3f("scale", 1, 1, 1);
		CShaderHolder::s_pInstance->GetShaderProgramById("model")->setUniform4f("rotation", foolme, 1, 0, 0);
		if (direction)
		{
			foolme += 0.1F;
		}
		else
		{
			foolme -= 0.1F;
		}
		if (foolme > 10) direction = false;
		if (foolme < -40) direction = true;
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glBindVertexArray(m_data.m_vertexArrayObject);
		glDrawElements(GL_TRIANGLES, m_data.m_numberOfIndexes, GL_UNSIGNED_SHORT, (void*)(0));

		err = glGetError();
		if (err != 0)
		{
			printf("glError Drawing Model = %d\n", err);
		}

		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		

		CShaderHolder::s_pInstance->StopShader();

		err = glGetError();
		if (err != 0)
		{
			printf("glError Drawing Model =%d\n", err);
		}
		
	}

	glDisable(GL_LIGHTING);
	glPopMatrix();


	glRotatef(rotate, 0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0, -1000);
	glVertex3f(100, 50, -1000);
	glVertex3f(640 / 2, 0, -1000);
	glEnd();

	glutSwapBuffers();
}

