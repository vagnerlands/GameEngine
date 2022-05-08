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

#include "Serializer/SerializationRecipe.h"
#include <filesystem>

bool 
EngineCore::IGame::Create()
{
	IGame::mGame = std::make_shared<Game>();
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

	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(mControls.m_ambient.GetX(), mControls.m_ambient.GetY(), mControls.m_ambient.GetZ()));


	// try to load the level_1 file
	const std::filesystem::path level_str = "Assets/level_1.json";
	if (!std::filesystem::exists(level_str))
		throw std::runtime_error("file not found: " + level_str.generic_string());

	std::ifstream file(level_str, std::ios::in | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	if (file.bad())
	{
		throw std::runtime_error("file is corrupted: " + level_str.generic_string());
	}
	std::vector<char> buffer(size);
	file.read(buffer.data(), size);
	JsonLevelData config;
	TryToConvert(buffer.data(), config);	

	// [Models]

	for (const auto& model : config.Models)
	{
		Graphics::RenderScene::Instance().Add(model.Name,
			CModelHolder::s_pInstance->GetModelById(model.ModelId),
			static_cast<Types::eSceneItemType>(model.sceneType))
			.SetScale(IvVector3{ model.scale })
			.SetLocation(IvVector3{ model.position })
			.SetTextureUV(IvVector2{ model.texture });
	}

	// [SkyBox]
	Graphics::RenderScene::Instance().Add("SKY1", UtilitiesCore::Skybox::CreateSky("sky1", config.SkyBox), eSceneItemType_SkyBox);

	// [Background Music]
	CSoundHolder::s_pInstance->PlaySoundById(config.BackgroundMusic[0].Name, eSoundType_Music, config.BackgroundMusic[0].Volume);

	Graphics::RenderScene::Instance().Add("Particles2", CParticlesSystemHolder::s_pInstance->GetParticleById("basic"), eSceneItemType_ParticlesSystem)
        .SetLocation(IvVector3(0.f, 100.0f, 200.5f))
        .SetCastShadows(false);

	Graphics::SceneQueryParticles part;
	part.Set(Graphics::ParticleSeeds(300, 100, 150, 10000, 12000, 10000, 12000, 50, 100, 100, 200, "flame.png"));
	Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);

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
    Graphics::TextRenderer::Instance().Add("frame_rate", string("FPS ") += std::to_string(GetFPS()), IvVector2(20.f, 0.f), 0.5f, IvVector3(0.f, 1.f, 0.f));
}

