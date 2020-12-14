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
#include "IClock.h"

#include "GL/glut.h"
#include "CGameController.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

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
	Graphics::IRenderer::mRenderer->GetCamera().MoveRight(5.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveForward(10.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(5.f);
	Graphics::IRenderer::mRenderer->GetCamera().RotateY(90.f);
	// Update this camera type
	Graphics::IRenderer::mRenderer->GetCamera().m_type = Camera_Spectator;
	// create model holder
	CModelHolder::s_pInstance->Create(".\\Assets\\model.zip");
    // 50mb allocation for VRAM textures
	CTextureHolder::s_pInstance->Create(".\\Assets\\textures.zip", 200U*1024U*1024U);

	IGame::mGame->SetFps(120);

	
	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	// [Models]

	Graphics::RenderScene::Instance().Add("Yoni1", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
	//Graphics::RenderScene::Instance().Add("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
	//Graphics::RenderScene::Instance().Add("Yoni3", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
	//Graphics::RenderScene::Instance().Add("Yoni4", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));

	Graphics::IModel* p = CModelHolder::s_pInstance->GetModelById("Cube.obj");
	//p->AppendTexture("brick_n.bmp", "normalMap");
	//p->AppendTexture("brick_n.bmp", "normalMap");
	Graphics::RenderScene::Instance().Add("Ground1", p);
	//Graphics::RenderScene::Instance().Add("castle1",	CModelHolder::s_pInstance->GetModelById("Castle OBJ.obj"));
    //Graphics::RenderScene::Instance().Add("church",	CModelHolder::s_pInstance->GetModelById("Church-scene.obj"));
	Graphics::RenderScene::Instance().Add("cyborg1",	CModelHolder::s_pInstance->GetModelById("cyborg.obj"));
	Graphics::RenderScene::Instance().Add("lightDebug", CModelHolder::s_pInstance->GetModelById("cube.obj"));
	// debug


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

	//Graphics::RenderScene::Instance().Translate("church",	IvVector3(0.f, 0.0f, -2.f));
	Graphics::RenderScene::Instance().Translate("cyborg1",	IvVector3(0.f, 0.5f, 5.f));
	Graphics::RenderScene::Instance().Rotate("cyborg1", IvQuat(IvVector3(0, 1, 0), 125));	
	Graphics::RenderScene::Instance().CastShadow("lightDebug", false);
	Graphics::RenderScene::Instance().Scale("lightDebug", IvVector3(0.3, 0.3, 0.3));
	//Graphics::RenderScene::Instance().Scale("castle1", IvVector3(1.3, 1.0, 1.3));
	Graphics::RenderScene::Instance().Scale("Ground1", IvVector3(100.0, 1.0, 100.0 ));

	Graphics::RenderScene::Instance().Scale("Yoni1", IvVector3(.020f, .020f, .020f));
	Graphics::RenderScene::Instance().Scale("Yoni2", IvVector3(.020f, .020f, .020f));
	Graphics::RenderScene::Instance().Scale("Yoni3", IvVector3(.020f, .020f, .020f));
	Graphics::RenderScene::Instance().Scale("Yoni4", IvVector3(.020f, .020f, .020f));

	Graphics::RenderScene::Instance().Translate("Yoni1", IvVector3(0.0, 1., 0.5));
	Graphics::RenderScene::Instance().Translate("Yoni2", IvVector3(3.0, 1., 3.5));
	Graphics::RenderScene::Instance().Translate("Yoni3", IvVector3(-3.0, 1., 3.5));
	Graphics::RenderScene::Instance().Translate("Yoni4", IvVector3(2.0, 1., 2.5));

	Graphics::RenderScene::Instance().Scale("SKY1", IvVector3(50,50,50));

	// [Landscape]


	return true;
}

void Game::ExecuteBackground()
{
	// Update objects based on current timestamp
	CModelHolder::s_pInstance->Update(mClock->GetTimeInMili());
}

void Game::UpdateObjects(float dt)
{
    const Float cSpeed = 15.0f;
	const Float cMouseSensibility = 5.f;
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
        CCamera& a = Graphics::IRenderer::mRenderer->GetCamera();
		noFlood = 0;
		//cout << "    X [" << a.m_position.GetX()
		//	<< "]   Y [" << a.m_position.GetY()
		//	<< "]   Z [" << a.m_position.GetZ() << "]" << endl;
	}
	// Update Debug objects
	// [Light]
	m_lightAngle += 0.35f;
	static float MoveRadius = 7.F;
    IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius + -10.1136, 7, cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius+5.65836);
	//IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius, 5.f, (cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius) - 20.f);
	Graphics::Ilumination::Instance().Update("main", lightLocation);
	Graphics::RenderScene::Instance().Translate("lightDebug", lightLocation);

}

void Game::Render(float dt)
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

	// first pass, using the shadows depth shader
	Graphics::Ilumination::Instance().StartShadowsDepth();
	Graphics::RenderScene::Instance().Render(dt, true);
	Graphics::Ilumination::Instance().FinishShadowsDepth();


	// adjust camera projection and view according to the current 
	// frustum parameters (3d - perspective mode)
	Graphics::IRenderer::mRenderer->PrepareCamera3D();	
	// Render everything in the scene database
	Graphics::RenderScene::Instance().Render(dt, false);

	glutSwapBuffers();
}

