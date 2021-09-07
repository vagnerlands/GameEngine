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
#include "Rendered/Skybox.h"
#include "IClock.h"
#include "OpenGL/Window2DOGL.h"

#include "GL/glut.h"
#include "CGameController.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "CSoundHolder.h"
#include "Scene/Query/SceneQueryParticles.h"
#include "Rendered/SceneItemBoundingBox.h"
#include "RenderUI.h"
#include "TextRenderer.h"

using namespace _Keys;

bool 
EngineCore::IGame::Create()
{
	IGame::mGame = new Game();
	return (IGame::mGame != nullptr);
}

Game::Game() : 
	EngineCore::IGame(),
    m_lightAngle(0.F),
    m_bias(10.6F),
    m_shadowDetailsFactor(0.59F),
    m_ambient(.5F, .5F, .5F)
{
    auto window = Graphics::RenderUI::Instance().Add("logger", std::make_shared<Graphics::Window2DOGL>(IvVector4(1.f,1.f, 1.f, 1.f), "window.bmp"));
    window->SetLocation({ 10.f, 10.f }).SetSize({ 400.f, 500.f });

	mpGameInput = new CGameController(1280, 720);
    KeyDispatcherFactory::Create(mpGameInput);
	BindKey(Key0, this);
	BindKey(Key1, this);
	BindKey(Key2, this);
	BindKey(Key3, this);
	BindKey(Keyt, this);
	BindKey(Keyy, this);
	BindKey(Keyg, this);
	BindKey(Keyh, this);
    BindKey(Keyv, this);
    BindKey(Keyb, this);
    BindKey(Keyn, this);
    BindKey(Keym, this);
    BindKey(Keyz, this);
    BindKey(Keyx, this);
    BindKey(Keyc, this);
    BindKey(KeyEscape, this);
}

Game::~Game()
{
	// release all allocated resources
    delete CParticlesSystemHolder::s_pInstance;    
    delete CTextureHolder::s_pInstance;
	delete CShaderHolder::s_pInstance;
	delete CModelHolder::s_pInstance;
    Graphics::RenderScene::Instance().Destroy();
}

bool Game::PostRendererInitialize()
{
	//TODO: initialize lighting default values (some basic light somewhere)
	CShaderHolder::Create();

    const Float cInitialSpeed = 600.f;
    const Float cAcceleration = 100.5f;
    const Float cMaxSpeed = 2000.f;    

    KeyDispatcher::Instance().Bind('w', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveForward(dt*(clamp(cInitialSpeed+(cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('s', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveForward(-dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('d', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveRight(dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('a', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveRight(-dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('e', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('q', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(-dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    Graphics::IRenderer::mRenderer->SetBias(m_bias);
    Graphics::IRenderer::mRenderer->SetShadowFactor(m_shadowDetailsFactor);
	// Set the view parameters in renderer
	Graphics::IRenderer::mRenderer->SetFOV(90.0F);
	// Update camera to be above ground
	Graphics::IRenderer::mRenderer->GetCamera().MoveRight(5.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveForward(1000.f);
	Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(180.f);
	Graphics::IRenderer::mRenderer->GetCamera().RotateY(90.f);
	// Update this camera type
	Graphics::IRenderer::mRenderer->GetCamera().SetCameraType(ECameraType::Camera_Spectator);
	// create model holder
	CModelHolder::s_pInstance->Create(".\\Assets\\model.zip");
	CParticlesSystemHolder::s_pInstance->Create(".\\Assets\\model.zip");
	CSoundHolder::s_pInstance->Create("test", 1);
    // allocation for VRAM textures
    const UInt32 cTextureCacheSizeInMBs = 120;
	CTextureHolder::s_pInstance->Create(".\\Assets\\textures.zip", cTextureCacheSizeInMBs*1024U*1024U);

	IGame::mGame->SetMaxFPS(120);

	
	// test sound
	CSoundHolder::s_pInstance->PlaySoundById("windhowl.wav", eSoundType_Effect, 0.1f);
	//CSoundHolder::s_pInstance->PlaySoundById("highlands.wav", eSoundType_Music, 0.5f);
	// Build a debug scenario
	// [Light]
	Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
	Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));

	// [Models]
	Graphics::RenderScene::Instance().Add("Ground1", CModelHolder::s_pInstance->GetModelById("Cube.obj"), eSceneItemType_Simple)
        .SetScale(IvVector3(20000.0, 1.0, 20000.0))
        .SetTextureUV(IvVector2(100.f, 100.f));

	//Graphics::RenderScene::Instance().Add("lightDebug", CModelHolder::s_pInstance->GetModelById("Cube.obj"), eSceneItemType_Simple)
    //       .SetScale(IvVector3(0.01f, 0.01f, 0.01f))
    //       .SetCastShadows(false);

    Graphics::RenderScene::Instance().Add("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"), eSceneItemType_AnimatedAndShadowed)
        .SetScale(IvVector3(1.f, 1.f, 1.f))
        .SetLocation(IvVector3(200.0, 0.5, 0));

    //Graphics::RenderScene::Instance().Add("Yoni1", CModelHolder::s_pInstance->GetModelById("nemesis.dae"), eSceneItemType_AnimatedAndShadowed)
    //    .SetScale(IvVector3(1.f, 1.f, 1.f))
    //    .SetLocation(IvVector3(-200.0, 0.5, 0));

    //Graphics::RenderScene::Instance().Add("Yoni3", CModelHolder::s_pInstance->GetModelById("Warrior.dae"), eSceneItemType_AnimatedAndShadowed)
    //    .SetScale(IvVector3(1.f, 1.f, 1.f))
    //    .SetLocation(IvVector3(500.0, 0.5, 0));

	Graphics::RenderScene::Instance().Add("Particles2", CParticlesSystemHolder::s_pInstance->GetParticleById("basic"), eSceneItemType_ParticlesSystem)
        .SetLocation(IvVector3(0.f, 100.0f, 200.5f))
        .SetCastShadows(false);

	Graphics::SceneQueryParticles part;
	part.Set(Graphics::ParticleSeeds(300, 100, 150, 10000, 12000, 10000, 12000, 50, 100, 100, 200, "flame.png"));
	Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);

	//CSoundHolder::s_pInstance->PlaySoundById("door.wav", eSoundType_Effect, 0.1f);
	//CSoundHolder::s_pInstance->PlaySoundById("windhowl.wav", eSoundType_Effect, 0.1f);
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
    Graphics::RenderScene::Instance().Add("SKY1", UtilitiesCore::Skybox::CreateSky("sky1", faces), eSceneItemType_SkyBox);
	// [Landscape]
    // TODO...

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

    // update all listening components about the pressed key (if any)
    for (Int32 key = 0; key < 256; ++key)
    {
        if (mpGameInput->m_bKey[key].isPressed)
        {
            KeyDispatcher::Instance().Event(dt, key);
        }
    }
    
    if (mpGameInput->m_bKey['y'].isPressed)
    {
        m_lightAngle += 1.f;
    }
    else if (mpGameInput->m_bKey['t'].isPressed)
    {
        m_lightAngle -= 1.f;
    }

	static Float SpreadConst = 100;
	static Float HeightConst = 10000;
	static Float SizeConst = 12000;
	static Float NumberOfParticles = 100;

	if (mpGameInput->m_bKey['o'].isPressed)
	{
		SpreadConst += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['p'].isPressed)
	{
		SpreadConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}

	if (mpGameInput->m_bKey['k'].isPressed)
	{
		HeightConst += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['l'].isPressed)
	{
		HeightConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	if (mpGameInput->m_bKey['n'].isPressed)
	{
		SizeConst += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['m'].isPressed)
	{
		SizeConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	
	if (mpGameInput->m_bKey['1'].isPressed)
	{
		NumberOfParticles -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mpGameInput->m_bKey['2'].isPressed)
	{
		NumberOfParticles += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
    if (mpGameInput->m_bKey['c'].isPressed)
    {
        Graphics::IRenderer::mRenderer->SetShadowFactor(m_shadowDetailsFactor += .1F);
        std::cout << "[ DEBUG ] Shadow details factor value " << m_shadowDetailsFactor << std::endl;
    }
    if (mpGameInput->m_bKey['v'].isPressed)
    {
        Graphics::IRenderer::mRenderer->SetShadowFactor(m_shadowDetailsFactor -= .1F);
        std::cout << "[ DEBUG ] Shadow details factor value " << m_shadowDetailsFactor << std::endl;
    }


    if (mpGameInput->m_bKey['g'].isPressed)
    {
        Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -.000001F);
    }
    if (mpGameInput->m_bKey['h'].isPressed)
    {
        Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", +.000001F);
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

	// Update Debug objects
	// [Light]
	m_lightAngle += 0.15f;
	static float MoveRadius = 1000.F;
    IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius, 
        500, 
        cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius);

    Graphics::RenderScene::Instance().Translate("Particles2", lightLocation);
	Graphics::Ilumination::Instance().Update("main", lightLocation);
	Graphics::RenderScene::Instance().Translate("lightDebug", lightLocation);

}

void Game::OnKeyEvent(const Keyt& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior_Attack.dae"));
    Graphics::RenderScene::Instance().DisplayBoundingBox("Yoni2", true);
}

void Game::OnKeyEvent(const Keyy& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
}

void Game::OnKeyEvent(const Keyg& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", 0.000005f);
}

void Game::OnKeyEvent(const Keyh& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -0.000005f);
}

void Game::OnKeyEvent(const KeyEscape & e)
{
    mGame->Quit();
}

void Game::OnKeyEvent(const Keyv & e)
{
    Graphics::RenderScene::Instance().Translate("Yoni2", IvVector3(1, 1, 0));
}

void Game::OnKeyEvent(const Keyb & e)
{
    Graphics::RenderScene::Instance().Translate("Yoni2", IvVector3(-1, 1, 0));
}

void Game::OnKeyEvent(const Keyn & e)
{
    Graphics::RenderScene::Instance().Scale("Yoni2", IvVector3(5.f, 5.f, 5.f));
}

void Game::OnKeyEvent(const Keym & e)
{
    Graphics::RenderScene::Instance().Scale("Yoni2", IvVector3(1.f, 1.f, 1.f));
}

void Game::OnKeyEvent(const Keyz& e)
{
    m_ambient -= 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void Game::OnKeyEvent(const Keyx& e)
{
    m_ambient += 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void Game::OnKeyEvent(const Keyc& e)
{
    //Graphics::IRenderer::mRenderer->SetBias(m_bias += 1000.0f);
    //std::cout << "[ DEBUG ] Light bias value " << m_bias << std::endl;
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

	// first pass, using the shadows depth shader
	Graphics::Ilumination::Instance().StartShadowsDepth();
	Graphics::RenderScene::Instance().Render(dt, true);
	Graphics::Ilumination::Instance().FinishShadowsDepth();


	// adjust camera projection and view according to the current 
	// frustum parameters (3d - perspective mode)
	Graphics::IRenderer::mRenderer->PrepareCamera3D();	
	// Render everything in the scene database
	Graphics::RenderScene::Instance().Render(dt, false);

    Graphics::IRenderer::mRenderer->PrepareCamera2D();
    Graphics::RenderUI::Instance().Render(dt);
    Graphics::TextRenderer::Instance().Render(string("FPS ") += std::to_string(GetFPS()), 12, 470, 0.25f, IvVector3(0, 1, 0));

	glutSwapBuffers();
}

