// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CParticlesSystemHolder.h"
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

#include "CSoundHolder.h"
#include "Scene/Query/SceneQueryParticles.h"

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
	BindKey(Key0, this);
	BindKey(Key1, this);
	BindKey(Key2, this);
	BindKey(Key3, this);
	BindKey(KeyT, this);
	BindKey(KeyY, this);
	BindKey(KeyG, this);
	BindKey(KeyH, this);
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
	CParticlesSystemHolder::s_pInstance->Create(".\\Assets\\model.zip");
	CSoundHolder::s_pInstance->Create("test", 1);
    // 50mb allocation for VRAM textures
	CTextureHolder::s_pInstance->Create(".\\Assets\\textures.zip", 100U*1024U*1024U);

	IGame::mGame->SetFps(120);

	
	// test sound
	//CSoundHolder::s_pInstance->PlaySoundById("windhowl.wav", eSoundType_Effect, 0.1f);
	CSoundHolder::s_pInstance->PlaySoundById("highlands.wav", eSoundType_Music, 0.5f);
	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(.5f, .5f, .5f));

	// [Models]
	Graphics::RenderScene::Instance().Add("Ground1", CModelHolder::s_pInstance->GetModelById("Cube.obj"), eSceneItemType_Simple);
	Graphics::RenderScene::Instance().Add("lightDebug", CModelHolder::s_pInstance->GetModelById("Cube.obj"), eSceneItemType_Simple);
	Graphics::RenderScene::Instance().CastShadow("lightDebug", false);

	Graphics::RenderScene::Instance().Add("Yoni1", CModelHolder::s_pInstance->GetModelById("nemesis.dae"), eSceneItemType_AnimatedAndShadowed);
	Graphics::RenderScene::Instance().Scale("Yoni1", IvVector3(.020f, .020f, .020f));
	Graphics::RenderScene::Instance().Translate("Yoni1", IvVector3(0, 0.5, 0));

	Graphics::RenderScene::Instance().Add("Particles2", CParticlesSystemHolder::s_pInstance->GetParticleById("basic"), eSceneItemType_ParticlesSystem);
	Graphics::RenderScene::Instance().Translate("Particles2", IvVector3(0.f, 1.0f, 2.5f));
	Graphics::RenderScene::Instance().CastShadow("Particles2", false);

	Graphics::SceneQueryParticles part;
	part.Set(Graphics::ParticleSeeds(300, 100, 150, 120, 420, 50, 250, 50, 100, 100, 200, "flame.png"));
	Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);

	//CSoundHolder::s_pInstance->PlaySoundById("door.wav", eSoundType_Effect, 0.1f);
	CSoundHolder::s_pInstance->PlaySoundById("windhowl.wav", eSoundType_Effect, 0.1f);
	//CSoundHolder::s_pInstance->PlaySoundById("door.wav", eSoundType_Effect, 0.1f);
	// debug


	// 6 faces of the sky - external interface must provide these
	vector<std::string> faces;
	faces.push_back("skyright.bmp");
	faces.push_back("skyleft.bmp");
	faces.push_back("skybottom.bmp");	
	faces.push_back("skytop.bmp");
	faces.push_back("skyfront.bmp");
	faces.push_back("skyback.bmp");
	Graphics::RenderScene::Instance().Add("SKY1",		new UtilitiesCore::Skybox("sky1", faces), eSceneItemType_SkyBox);
	// update models location

	//Graphics::RenderScene::Instance().Translate("church",	IvVector3(0.f, 0.0f, -2.f));
	Graphics::RenderScene::Instance().Translate("cyborg1",	IvVector3(0.f, 0.5f, 5.f));
	Graphics::RenderScene::Instance().Rotate("cyborg1", IvQuat(IvVector3(0, 1, 0), 125));	
	Graphics::RenderScene::Instance().CastShadow("lightDebug", false);
	Graphics::RenderScene::Instance().Scale("lightDebug", IvVector3(0.3, 0.3, 0.3));
	//Graphics::RenderScene::Instance().Scale("castle1", IvVector3(1.3, 1.0, 1.3));
	Graphics::RenderScene::Instance().Scale("Ground1", IvVector3(200.0, 1.0, 200.0));
	Graphics::RenderScene::Instance().SetTextureUV("Ground1", IvVector2(10.f, 10.f));

	Graphics::RenderScene::Instance().Scale("SKY1", IvVector3(50,50,50));

	// [Landscape]
	

	return true;
}

void Game::ExecuteBackground()
{	
	// Update objects based on current timestamp
	CModelHolder::s_pInstance->Update(mClock->GetTimeInMili());
	//Graphics::RenderScene::Instance().Update(mClock->GetTimeInMili());
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

	static Float SpreadConst = 10;
	static Float HeightConst = 100;
	static Float SizeConst = 0;
	static Float NumberOfParticles = 100;

	if (mpGameInput->m_bKey['o'])
	{
		SpreadConst += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 300 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['p'])
	{
		SpreadConst -= 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}

	if (mpGameInput->m_bKey['k'])
	{
		HeightConst += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['l'])
	{
		HeightConst -= 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200+ SizeConst, 500+ SizeConst, 200+HeightConst, 400 + HeightConst, 30+SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	if (mpGameInput->m_bKey['n'])
	{
		SizeConst += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['m'])
	{
		SizeConst -= 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	
	if (mpGameInput->m_bKey['1'])
	{
		NumberOfParticles -= 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['2'])
	{
		NumberOfParticles += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 100, 150, 200 + SizeConst, 500 + SizeConst, 200 + HeightConst, 400 + HeightConst, 30 + SpreadConst, 70 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
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
	m_lightAngle += 0.15f;
	static float MoveRadius = 15.F;
    IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius + -10.1136, 20, cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius+5.65836);
	//IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius, 5.f, (cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius) - 20.f);
	Graphics::Ilumination::Instance().Update("main", lightLocation);
	Graphics::RenderScene::Instance().Translate("lightDebug", lightLocation);

}

void Game::OnKeyEvent(const KeyT& e)
{
	Graphics::RenderScene::Instance().Change("Yoni1", CModelHolder::s_pInstance->GetModelById("Warrior_Taunt.dae"));
}

void Game::OnKeyEvent(const KeyY& e)
{
	Graphics::RenderScene::Instance().Change("Yoni1", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
}

void Game::OnKeyEvent(const KeyG& e)
{
	Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", 0.015f);
}

void Game::OnKeyEvent(const KeyH& e)
{
	Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -0.015f);
}

void Game::OnKeyEvent(const Key0& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 1, 1));
}

void Game::OnKeyEvent(const Key1& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1,0,0));
}

void Game::OnKeyEvent(const Key2& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(0, 1, 0));
}

void Game::OnKeyEvent(const Key3& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 0, 1));
}


void Game::Render(float dt)
{
	// set which buffers are used, set the modelview matrix and more
	Graphics::IRenderer::mRenderer->PrepareFrame();
	// must be called once in a while
	CModelHolder::s_pInstance->Refresh();

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

