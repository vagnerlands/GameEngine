// Game.cpp : Defines the entry point for the console application.
//

#include "Game.h"
#include "IRenderer.h"
#include "CModelHolder.h"
#include "CParticlesSystemHolder.h"
#include "IClock.h"

// Rendering Scene
#include "Scene/RenderScene.h"
#include "Scene/Ilumination.h"
// debug
#include "Rendered/Skybox.h"

#include "CSoundHolder.h"
#include "Scene/Query/SceneQueryParticles.h"
#include "RenderUI.h"
#include "TextRenderer.h"

bool 
EngineCore::IGame::Create()
{
	IGame::mGame = new Game();
	return (IGame::mGame != nullptr);
}

Game::Game() :
    EngineCore::IGame(),
    mControls(this, IGame::mGameController)
{}

Game::~Game()
{
    Graphics::RenderScene::Instance().Destroy();
}

bool Game::PostRendererInitialize()
{
    Graphics::IRenderer::mRenderer->SetBias(mControls.m_bias);
    Graphics::IRenderer::mRenderer->SetShadowFactor(mControls.m_shadowDetailsFactor);
	// Set the view parameters in renderer
	Graphics::IRenderer::mRenderer->SetFOV(90.0F);
	// Update camera to be above ground
	Graphics::IRenderer::mRenderer->GetCamera().MoveRight(500.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveForward(1000.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(180.f);
	Graphics::IRenderer::mRenderer->GetCamera().RotateY(90.f);
	// Update this camera type
	Graphics::IRenderer::mRenderer->GetCamera().SetCameraType(ECameraType::Camera_Spectator);

	IGame::mGame->SetMaxFPS(120);
	
	// test sound
	CSoundHolder::s_pInstance->PlaySoundById("windhowl.wav", eSoundType_Effect, 0.1f);
	//CSoundHolder::s_pInstance->PlaySoundById("highlands.wav", eSoundType_Music, 0.5f);
	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(mControls.m_ambient.GetX(), mControls.m_ambient.GetY(), mControls.m_ambient.GetZ()));

	// [Models]
	Graphics::RenderScene::Instance().Add("Ground1", CModelHolder::s_pInstance->GetModelById("Cube.obj"), eSceneItemType_Simple)
        .SetScale(IvVector3(20000.0, 1.0, 20000.0))
        .SetTextureUV(IvVector2(100.f, 100.f));

    Graphics::RenderScene::Instance().Add("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"), eSceneItemType_AnimatedAndShadowed)
        .SetScale(IvVector3(1.f, 1.f, 1.f))
        .SetLocation(IvVector3(5000.0, 0.5, 0));

    Graphics::RenderScene::Instance().Add("Nemesis1", CModelHolder::s_pInstance->GetModelById("nemesis.dae"), eSceneItemType_AnimatedAndShadowed)
        .SetScale(IvVector3(1.f, 1.f, 1.f))
        .SetLocation(IvVector3(-5000.0, 0.5, 0));

    Graphics::RenderScene::Instance().Add("Warrior1", CModelHolder::s_pInstance->GetModelById("Warrior_Attack.dae"), eSceneItemType_AnimatedAndShadowed)
        .SetScale(IvVector3(1.f, 1.f, 1.f))
        .SetLocation(IvVector3(0.f, 0.5, 5000.f));

	Graphics::RenderScene::Instance().Add("Particles2", CParticlesSystemHolder::s_pInstance->GetParticleById("basic"), eSceneItemType_ParticlesSystem)
        .SetLocation(IvVector3(0.f, 100.0f, 200.5f))
        .SetCastShadows(false);

	Graphics::SceneQueryParticles part;
	part.Set(Graphics::ParticleSeeds(300, 100, 150, 10000, 12000, 10000, 12000, 50, 100, 100, 200, "flame.png"));
	Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);

	// 6 faces of the sky - external interface must provide these
    vector<std::string> faces = { "skyright.bmp" , "skyleft.bmp", "skybottom.bmp", "skytop.bmp", "skyfront.bmp", "skyback.bmp" };
    Graphics::RenderScene::Instance().Add("SKY1", UtilitiesCore::Skybox::CreateSky("sky1", faces), eSceneItemType_SkyBox);
	// [Landscape]
    // TODO...

	return true;
}

void Game::ExecuteBackground()
{	
    IGame::ExecuteBackground();
	// Update objects based on current timestamp
	CModelHolder::s_pInstance->Update(mClock->GetTimeInMili());
}

void Game::UpdateObjects(float dt)
{
    mControls.Execute(dt);
}

void Game::Render(float dt)
{
    IGame::Render(dt);    
    IGame::PostRender();	
    Graphics::TextRenderer::Instance().Add("frame_rate", string("FPS ") += std::to_string(GetFPS()), 12.f, 0.f, 0.25f, IvVector3(0.f, 1.f, 0.f));
}

