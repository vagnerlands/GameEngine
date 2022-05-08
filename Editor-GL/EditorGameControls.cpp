#include "EditorGameControls.h"
#include "CGameController.h"
#include "IRenderer.h"
#include "Query/SceneQueryParticles.h"
#include "RenderScene.h"
#include "Ilumination.h"
#include "IGame.h"

using namespace _Keys;
EditorGameControls::EditorGameControls(EngineCore::IGame* pGameEngine, CGameController& pController) :
    mpGameEngine(pGameEngine),
    mGameInput(pController),
    m_lightAngle(0.F),
    m_bias(10.6F),
    m_shadowDetailsFactor(0.59F),
    m_ambient(.5F, .5F, .5F)
{
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

EditorGameControls::~EditorGameControls()
{}

CGameController * EditorGameControls::GetGameController()
{
    return &mGameInput;
}


void EditorGameControls::Execute(float dt)
{
    const Float cSpeed = 15.0f;
	const Float cMouseSensibility = 5.f;

    // must be called cyclically to activate any events
    mGameInput.OnUpdate(dt);
    
    if (mGameInput.m_bKey['y'].isPressed)
    {
        m_lightAngle += 1.f;
    }
    else if (mGameInput.m_bKey['t'].isPressed)
    {
        m_lightAngle -= 1.f;
    }

	static Float SpreadConst = 100;
	static Float HeightConst = 10000;
	static Float SizeConst = 12000;
	static Float NumberOfParticles = 100;

	if (mGameInput.m_bKey['o'].isPressed)
	{
		SpreadConst += 1.f;
		Graphics::SceneQueryParticles part;
		part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mGameInput.m_bKey['p'].isPressed)
	{
		SpreadConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}

	if (mGameInput.m_bKey['k'].isPressed)
	{
		HeightConst += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mGameInput.m_bKey['l'].isPressed)
	{
		HeightConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	if (mGameInput.m_bKey['n'].isPressed)
	{
		SizeConst += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mGameInput.m_bKey['m'].isPressed)
	{
		SizeConst -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	
	if (mGameInput.m_bKey['1'].isPressed)
	{
		NumberOfParticles -= 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
	else if (mGameInput.m_bKey['2'].isPressed)
	{
		NumberOfParticles += 1.f;
		Graphics::SceneQueryParticles part;
        part.Set(Graphics::ParticleSeeds(NumberOfParticles, 300, 150, SizeConst, 5000 + SizeConst, 2000 + HeightConst, 3000 + HeightConst, 30, 40 + SpreadConst, 100, 200, "flame.png"));
		Graphics::RenderScene::Instance().ApplyQuery("Particles2", part);
	}
    if (mGameInput.m_bKey['c'].isPressed)
    {
        Graphics::IRenderer::mRenderer->SetShadowFactor(m_shadowDetailsFactor += .1F);
        std::cout << "[ DEBUG ] Shadow details factor value " << m_shadowDetailsFactor << std::endl;
    }
    if (mGameInput.m_bKey['v'].isPressed)
    {
        Graphics::IRenderer::mRenderer->SetShadowFactor(m_shadowDetailsFactor -= .1F);
        std::cout << "[ DEBUG ] Shadow details factor value " << m_shadowDetailsFactor << std::endl;
    }


    if (mGameInput.m_bKey['g'].isPressed)
    {
        Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -.000001F);
    }
    if (mGameInput.m_bKey['h'].isPressed)
    {
        Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", +.000001F);
    }

	if (mGameInput.m_isLeftButtonPressed)
	{		
		const Float cosValue = cos((mGameInput.m_movementAngle * (3.14159 / 180.F)));
		const Float sinValue = sin((mGameInput.m_movementAngle * (3.14159 / 180.F)));
		//printf(" Intensity X %d Y %d cos %f sin %f\n", mGameInput.m_movementIntensityX, mGameInput.m_movementIntensityY, cosValue, sinValue);
		Graphics::IRenderer::mRenderer->GetCamera().RotateX(cosValue * abs(mGameInput.m_movementIntensityY) * dt * cMouseSensibility);
		Graphics::IRenderer::mRenderer->GetCamera().RotateY(sinValue * abs(-mGameInput.m_movementIntensityX) * dt * cMouseSensibility);
	}

	if (mGameInput.m_isRightButtonPressed)
	{
		const Float cosValue = cos((mGameInput.m_angleFromCenter * (3.14159 / 180.F)));
		const Float sinValue = sin((mGameInput.m_angleFromCenter * (3.14159 / 180.F)));

		const Float MovementIntensityOnX = clamp(0, mGameInput.m_distanceFromCenter - 50);
		const Float MovementIntensityOnY = clamp(0, mGameInput.m_distanceFromCenter - 50);

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

void EditorGameControls::OnKeyEvent(const Keyt& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior_Attack.dae"));
    Graphics::RenderScene::Instance().DisplayBoundingBox("Warrior1", true);
}

void EditorGameControls::OnKeyEvent(const Keyy& e)
{
	//Graphics::RenderScene::Instance().Change("Yoni2", CModelHolder::s_pInstance->GetModelById("Warrior.dae"));
}

void EditorGameControls::OnKeyEvent(const Keyg& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", 0.000005f);
}

void EditorGameControls::OnKeyEvent(const Keyh& e)
{
	//Graphics::Ilumination::Instance().IncreaseAttenuationBy("main", -0.000005f);
}

void EditorGameControls::OnKeyEvent(const KeyEscape & e)
{
    mpGameEngine->Quit();
}

void EditorGameControls::OnKeyEvent(const Keyv & e)
{
    Graphics::RenderScene::Instance().Translate("Warrior1", IvVector3(1, 1, 0));
}

void EditorGameControls::OnKeyEvent(const Keyb & e)
{
    Graphics::RenderScene::Instance().Translate("Warrior1", IvVector3(-1, 1, 0));
}

void EditorGameControls::OnKeyEvent(const Keyn & e)
{
    Graphics::RenderScene::Instance().Scale("Warrior1", IvVector3(5.f, 5.f, 5.f));
}

void EditorGameControls::OnKeyEvent(const Keym & e)
{
    Graphics::RenderScene::Instance().Scale("Warrior1", IvVector3(1.f, 1.f, 1.f));
}

void EditorGameControls::OnKeyEvent(const Keyz& e)
{
    m_ambient -= 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void EditorGameControls::OnKeyEvent(const Keyx& e)
{
    m_ambient += 0.05F;
    Graphics::Ilumination::Instance().SetAmbientLightColor(IvVector3(m_ambient.GetX(), m_ambient.GetY(), m_ambient.GetZ()));
    std::cout << "[ DEBUG ] Ambient Light is"
        << " X " << m_ambient.GetX()
        << " Y " << m_ambient.GetY()
        << " Z " << m_ambient.GetZ()
        << std::endl;
}

void EditorGameControls::OnKeyEvent(const Keyc& e)
{
    //Graphics::IRenderer::mRenderer->SetBias(m_bias += 1000.0f);
    //std::cout << "[ DEBUG ] Light bias value " << m_bias << std::endl;
}

void EditorGameControls::OnKeyEvent(const Key0& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 1, 1));
}

void EditorGameControls::OnKeyEvent(const Key1& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1,0,0));
}

void EditorGameControls::OnKeyEvent(const Key2& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(0, 1, 0));
}

void EditorGameControls::OnKeyEvent(const Key3& e)
{
	Graphics::Ilumination::Instance().SetLightColor("main", IvVector3(1, 0, 1));
}
