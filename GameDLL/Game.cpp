// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CShaderHolder.h"
#include "CTextureHolder.h"
#include "CThreadHolder.h"
#include "GL/glut.h"
#include "CGameController.h"

bool 
EngineCore::IGame::Create()
{
	IGame::mGame = new Game();
	return (IGame::mGame != nullptr);
}

Game::Game() : 
	EngineCore::IGame()
{
	mpGameInput = new CGameController(640, 480);
}

Game::~Game()
{
	// release all allocated resources
	delete CTextureHolder::s_pInstance;
	delete CShaderHolder::s_pInstance;
	delete CModelHolder::s_pInstance;
}

bool Game::PostRendererInitialize()
{
	//TODO: initialize lighting default values (some basic light somewhere)
	CShaderHolder::Create();

	// Set the view parameters in renderer
	Graphics::IRenderer::mRenderer->SetFOV(60.0F);
	// create model holder
	CModelHolder::s_pInstance->Create("..\\Game\\Assets\\model.zip");
    // 50MB of VRAM allocation
	CTextureHolder::s_pInstance->Create("..\\Game\\Assets\\textures.zip", 50U*1024U*1024U);

	CTextureHolder::s_pInstance->getTextureById("water.bmp");
	CTextureHolder::s_pInstance->getTextureById("brick_t.bmp");
	CTextureHolder::s_pInstance->getTextureById("brick_n.bmp");

	IGame::mGame->SetFps(60);

	return true;
}

void Game::UpdateObjects(float dt)
{
	if (mpGameInput->m_bKey[27])
	{
		mGame->Quit();
		return;
	}
	if (mpGameInput->m_bKey['W'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveForward(dt*1.0);
	} 
	else if (mpGameInput->m_bKey['S'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveForward(-dt*1.0);
	}
	if (mpGameInput->m_bKey['D'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveRight(dt*1.0);
	}
	else if (mpGameInput->m_bKey['A'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveRight(-dt*1.0);
	}

	if (mpGameInput->m_bKey['E'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(dt*1.0);
	}
	else if (mpGameInput->m_bKey['Q'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(-dt*1.0);
	}

	if (mpGameInput->m_isLeftButtonPressed)
	{		
		const Float cosValue = cos((mpGameInput->m_movementAngle * (3.14159 / 180.F)));
		const Float sinValue = sin((mpGameInput->m_movementAngle * (3.14159 / 180.F)));
		//printf(" Intensity X %d Y %d cos %f sin %f\n", mpGameInput->m_movementIntensityX, mpGameInput->m_movementIntensityY, cosValue, sinValue);
		Graphics::IRenderer::mRenderer->GetCamera().RotateX(cosValue * abs(mpGameInput->m_movementIntensityY) * dt * 10);
		Graphics::IRenderer::mRenderer->GetCamera().RotateY(sinValue * abs(mpGameInput->m_movementIntensityX) * dt * 10);
	}

	if (mpGameInput->m_isRightButtonPressed)
	{
		const Float cosValue = cos((mpGameInput->m_angleFromCenter * (3.14159 / 180.F)));
		const Float sinValue = sin((mpGameInput->m_angleFromCenter * (3.14159 / 180.F)));

		const Float MovementIntensityOnX = clamp(0, mpGameInput->m_distanceFromCenter - 50);
		const Float MovementIntensityOnY = clamp(0, mpGameInput->m_distanceFromCenter - 50);

		Graphics::IRenderer::mRenderer->GetCamera().RotateX(cosValue * abs(MovementIntensityOnY) * dt * 0.5f);
		Graphics::IRenderer::mRenderer->GetCamera().RotateY(sinValue * abs(MovementIntensityOnX) * dt * 0.5f);
	}
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
	
	GLuint textureId = -1;

	glPushMatrix();
	if (CShaderHolder::s_pInstance->UseShaderById("textured2"))
	{

		// light set-up
		glEnable(GL_LIGHT0);

		static float LightMoving = 2.F;
		static float Angle = 0.0;
		//LightMoving -= 0.001f;
		Angle += 1.0f;
		GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_direction[] = { 0.0, -1.0, 0.0 };
		GLfloat light_position[] = { sin(Angle * 3.14159 / 180.F) * LightMoving, 1, cos(Angle * 3.14159 / 180.F) * LightMoving - 2.f, 1.0f};

		glColor4f(1, 0, 0, 1);
		glBegin(GL_QUADS);
		glVertex3f(light_position[0] - 0.1f, light_position[1], light_position[2] - 0.1f);
		glVertex3f(light_position[0] - 0.1f, light_position[1], light_position[2] + 0.1f);
		glVertex3f(light_position[0] + 0.1f, light_position[1], light_position[2] + 0.1f);
		glVertex3f(light_position[0] + 0.1f, light_position[1], light_position[2] - 0.1f);
		glEnd();

		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		Int32 err = glGetError();
		if (err != 0)
		{
			printf("preparing matrix error = %d\n", err);
		}

		CShaderHolder::s_pInstance->GetShaderProgramById("textured2")->setUniform3f("translate", 0, 0.25, 0);
		CShaderHolder::s_pInstance->GetShaderProgramById("textured2")->setUniform3f("scale", 1, 1, 1);
		CShaderHolder::s_pInstance->GetShaderProgramById("textured2")->setUniform4f("rotation", 5.F /*+ foolme*/, 1, 0, 0);

		glEnable(GL_TEXTURE_2D);

		CShaderHolder::s_pInstance->GetShaderProgramById("textured2")->setTexture("textureColor", 
			CTextureHolder::s_pInstance->getTextureById("brick_t.bmp"));

		CShaderHolder::s_pInstance->GetShaderProgramById("textured2")->setTexture("textureNormal",
			CTextureHolder::s_pInstance->getTextureById("brick_n.bmp"));

		GLfloat mat_shininess[] = { 1.0, 0.5, 0.31 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_shininess);

		// draw blocks of 1 meter^2 and apply texture
		glBegin(GL_QUADS);
		const Int32 sizeOfBlock = 2;
		for (Int32 bx = -10; bx < 10; bx += sizeOfBlock)
		{
			for (Int32 by = 0; by > -20; by -= sizeOfBlock)
			{

				glTexCoord2f(0, 0);  glVertex3f(bx, -0.5, by);
				glTexCoord2f(1, 0);  glVertex3f(bx + sizeOfBlock, -0.5, by);
				glTexCoord2f(1, 1);  glVertex3f(bx + sizeOfBlock, -0.5, by + sizeOfBlock);
				glTexCoord2f(0, 1);  glVertex3f(bx, -0.5, by + sizeOfBlock);

			}
		}
		glEnd();

		CShaderHolder::s_pInstance->StopShader();
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}
	glPopMatrix();

	glDisable(GL_LIGHTING);


	// [ TEXTURED SQUARE ]
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	
	CTextureHolder::s_pInstance->Bind("brick_t.bmp");

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-200, -100, -1000); glTexCoord2f(0, 0);
	glVertex3f(100, -100, -1000); glTexCoord2f(0, 1);
	glVertex3f(100, 100, -1000); glTexCoord2f(1, 1);
	glVertex3f(-200, 100, -1000); glTexCoord2f(1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
	

	//glutSwapBuffers();
}

