// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CShaderHolder.h"
#include "CTextureHolder.h"
#include "CThreadHolder.h"

// Rendering Scene
#include "Scene\RenderScene.h"
#include "Scene\Ilumination.h"
// debug
#include "CModelOGL.h"
#include "Rendered/Skybox.h"

#include "GL/glut.h"
#include "CGameController.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "ShadowsOGL.h"

bool 
EngineCore::IGame::Create()
{
	IGame::mGame = new Game();
	return (IGame::mGame != nullptr);
}

Game::Game() : 
	EngineCore::IGame(),
    m_lightAngle(0.F)
{
	mpGameInput = new CGameController(1280, 720);
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
	// Update camera to be above ground
	Graphics::IRenderer::mRenderer->GetCamera().m_position.SetY(1.f);
	// Update this camera type
	Graphics::IRenderer::mRenderer->GetCamera().m_type = Camera_Spectator;
	// create model holder
	CModelHolder::s_pInstance->Create("..\\Game\\Assets\\model.zip");
    // 50mb allocation for VRAM textures
	CTextureHolder::s_pInstance->Create("..\\Game\\Assets\\textures.zip", 200U*1024U*1024U);

	IGame::mGame->SetFps(60);

	Graphics::Ilumination::Instance().Initialize(new Graphics::ShadowsOGL);

	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	// [Models]
	Graphics::RenderScene::Instance().Add("castle1",	CModelHolder::s_pInstance->GetModelById("Castle OBJ.obj"));
	Graphics::RenderScene::Instance().Add("cyborg1",	CModelHolder::s_pInstance->GetModelById("cyborg.obj"));
	Graphics::RenderScene::Instance().Add("ogre1",		CModelHolder::s_pInstance->GetModelById("OgreOBJ.obj"));
	// 6 faces of the sky - external interface must provide these
	vector<std::string> faces;
	faces.push_back("skyright.bmp");
	faces.push_back("skyleft.bmp");
	faces.push_back("skybottom.bmp");	
	faces.push_back("skytop.bmp");
	faces.push_back("skyfront.bmp");
	faces.push_back("skyback.bmp");
	Graphics::RenderScene::Instance().Add("SKY1",		new UtilitiesCore::Skybox("sky1", faces));
	// update models location
	Graphics::RenderScene::Instance().Translate("ogre1",	IvVector3(4, 1, 0));
	Graphics::RenderScene::Instance().Translate("cyborg1",	IvVector3(0, 1, 0));
	Graphics::RenderScene::Instance().Scale("SKY1", IvVector3(50,50,50));
	// [Landscape]


	return true;
}

void Game::UpdateObjects(float dt)
{
    const Float cSpeed = 10.f;
	const Float cMouseSensibility = 1.25f;
	if (mpGameInput->m_bKey[27])
	{
		mGame->Quit();
		return;
	}
	if (mpGameInput->m_bKey['w'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveForward(dt*cSpeed);
	} 
	else if (mpGameInput->m_bKey['s'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveForward(-dt* cSpeed);
	}
	if (mpGameInput->m_bKey['d'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveRight(dt*cSpeed);
	}
	else if (mpGameInput->m_bKey['a'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveRight(-dt* cSpeed);
	}

	if (mpGameInput->m_bKey['e'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(dt*cSpeed);
	}
	else if (mpGameInput->m_bKey['q'])
	{
		Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(-dt* cSpeed);
	}
    
    if (mpGameInput->m_bKey['y'])
    {
        m_lightAngle += 1.f;
    }
    else if (mpGameInput->m_bKey['t'])
    {
        m_lightAngle -= 1.f;
    }


	if (mpGameInput->m_isLeftButtonPressed)
	{		
		const Float cosValue = cos((mpGameInput->m_movementAngle * (3.14159 / 180.F)));
		const Float sinValue = sin((mpGameInput->m_movementAngle * (3.14159 / 180.F)));
		//printf(" Intensity X %d Y %d cos %f sin %f\n", mpGameInput->m_movementIntensityX, mpGameInput->m_movementIntensityY, cosValue, sinValue);
		Graphics::IRenderer::mRenderer->GetCamera().RotateX(cosValue * abs(mpGameInput->m_movementIntensityY) * dt * cMouseSensibility);
		Graphics::IRenderer::mRenderer->GetCamera().RotateY(sinValue * abs(-mpGameInput->m_movementIntensityX) * dt * cMouseSensibility);
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

	static int noFlood = 0;
	if (noFlood++ > 100)
	{
		noFlood = 0;
		CCamera a = Graphics::IRenderer::mRenderer->GetCamera();
		cout << "    X [" << a.m_position.GetX()
			<< "]   Y [" << a.m_position.GetY()
			<< "]   Z [" << a.m_position.GetZ() << "]" << endl;
	}

	// Update Debug objects
	// [Light]
	m_lightAngle += 1.115f;
	static float MoveRadius = 4.F;
	Graphics::Ilumination::Instance().Update("main", IvVector3(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius, 8.5f, cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius));

	Graphics::RenderScene::Instance().Rotate("ogre1", IvQuat(IvVector3(0, 1, 0), -15));

	Graphics::RenderScene::Instance().Rotate("cyborg1", IvQuat(IvVector3(0, 1, 0),	45));

	// [Model]

	// [Landscape]

}

void Game::Render()
{
	// set which buffers are used, set the modelview matrix and more
	Graphics::IRenderer::mRenderer->PrepareFrame();

	/*const Float aspect = Graphics::IRenderer::mRenderer->GetAspect();

	const CCamera& camera = Graphics::IRenderer::mRenderer->GetCamera();

	Graphics::IRenderer::mRenderer->SetOrtho(
		aspect * 3.0f + camera.m_position.GetX(), 
		-aspect*3.0f + camera.m_position.GetX(),
		3.0f + camera.m_position.GetY(),
		-3.0f + camera.m_position.GetY(),
		1, 
		10);
	Graphics::IRenderer::mRenderer->PrepareCamera2D();*/

	
	Graphics::IRenderer::mRenderer->PrepareShadows3D();
	Graphics::Ilumination::Instance().StartShadowsDepth();
	Graphics::RenderScene::Instance().Render(true);
	Graphics::Ilumination::Instance().FinishShadowsDepth();
	// adjust camera projection and view according to the current 
	// frustum parameters (3d - perspective mode)
	Graphics::IRenderer::mRenderer->PrepareCamera3D();
	
	// Render everything in the scene database
	Graphics::RenderScene::Instance().Render(false);

	glutSwapBuffers();
}

