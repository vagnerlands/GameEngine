#include "GameControls.h"
#include "CGameController.h"
#include "IRenderer.h"
#include "Query/SceneQueryParticles.h"
#include "RenderScene.h"
#include "Ilumination.h"
#include "IGame.h"

using namespace _Keys;
GameControls::GameControls(EngineCore::IGame* pGameEngine) : 
    mpGameEngine(pGameEngine),
    m_lightAngle(0.F),
    m_bias(10.6F),
    m_shadowDetailsFactor(0.59F),
    m_ambient(.5F, .5F, .5F)
{
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

    const Float cInitialSpeed = 600.f;
    const Float cAcceleration = 100.5f;
    const Float cMaxSpeed = 2000.f;

    KeyDispatcher::Instance().Bind('w', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveForward(dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
        //cout << "Distance from Camera to Yoni2 is << " << Distance(Graphics::IRenderer::mRenderer->GetCamera().m_position, Graphics::RenderScene::Instance().Find("Yoni2").GetLocation()) << endl;
    });

    KeyDispatcher::Instance().Bind('s', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveForward(-dt * (clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
        //cout << "Distance from Camera to Yoni2 is << " << Distance(Graphics::IRenderer::mRenderer->GetCamera().m_position, Graphics::RenderScene::Instance().Find("Yoni2").GetLocation()) << endl;
    });

    KeyDispatcher::Instance().Bind('d', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveRight(dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('a', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveRight(-dt * (clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('e', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(dt*(clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });

    KeyDispatcher::Instance().Bind('q', [=](Float dt, Int32 cycles)
    {
        Graphics::IRenderer::mRenderer->GetCamera().MoveUpward(-dt * (clamp(cInitialSpeed + (cycles*cAcceleration), cMaxSpeed)));
    });
}

GameControls::~GameControls()
{}

CGameController * GameControls::GetGameController()
{
    return mpGameInput;
}


void GameControls::Execute(float dt)
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

void GameControls::OnKeyEvent(const Keyt& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior_Attack.dae"));
    Graphics::RenderScene::Instance().DisplayBoundingBox("Yoni2", true);
}

void GameControls::OnKeyEvent(const Keyy& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
}

void GameControls::OnKeyEvent(const Keyg& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", 0.000005f);
}

void GameControls::OnKeyEvent(const Keyh& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -0.000005f);
}

void GameControls::OnKeyEvent(const KeyEscape & e)
{
    mpGameEngine->Quit();
}

void GameControls::OnKeyEvent(const Keyv & e)
{
    Graphics::RenderScene::Instance().Translate("Yoni2", IvVector3(1, 1, 0));
}

void GameControls::OnKeyEvent(const Keyb & e)
{
    Graphics::RenderScene::Instance().Translate("Yoni2", IvVector3(-1, 1, 0));
}

void GameControls::OnKeyEvent(const Keyn & e)
{
    Graphics::RenderScene::Instance().Scale("Yoni2", IvVector3(5.f, 5.f, 5.f));
}

void GameControls::OnKeyEvent(const Keym & e)
{
    Graphics::RenderScene::Instance().Scale("Yoni2", IvVector3(1.f, 1.f, 1.f));
}

void GameControls::OnKeyEvent(const Keyz& e)
{
    m_ambient -= 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void GameControls::OnKeyEvent(const Keyx& e)
{
    m_ambient += 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void GameControls::OnKeyEvent(const Keyc& e)
{
    //Graphics::IRenderer::mRenderer->SetBias(m_bias += 1000.0f);
    //std::cout << "[ DEBUG ] Light bias value " << m_bias << std::endl;
}

void GameControls::OnKeyEvent(const Key0& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 1, 1));
}

void GameControls::OnKeyEvent(const Key1& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1,0,0));
}

void GameControls::OnKeyEvent(const Key2& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(0, 1, 0));
}

void GameControls::OnKeyEvent(const Key3& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 0, 1));
}