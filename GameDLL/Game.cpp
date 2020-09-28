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
// Rendering Scene
#include "Scene\RenderScene.h"
#include "Scene\Ilumination.h"
#include "Rendered/Skybox.h"
// TODO: this should be abstract
#include "Scene/ShadowsOGL.h"


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

    // TODO: make this cross-platform
    Graphics::Ilumination::Instance().Initialize(new Graphics::ShadowsOGL);

    // Set the view parameters in renderer
    Graphics::IRenderer::mRenderer->SetFOV(60.0F);
    // Update camera to be above ground
    Graphics::IRenderer::mRenderer->GetCamera().m_position.SetY(1.f);
    // Update this camera type
    Graphics::IRenderer::mRenderer->GetCamera().m_type = Camera_Spectator;
    // create model holder
    CModelHolder::s_pInstance->Create("..\\Game\\Assets\\model.zip");
    // 50mb allocation for VRAM textures
    CTextureHolder::s_pInstance->Create("..\\Game\\Assets\\textures.zip", 200U * 1024U * 1024U);

    IGame::mGame->SetFps(60);

    // Build a debug scenario
    // [Light]
    Graphics::Ilumination::Instance().Add(new Graphics::IluminationItem("main", IvVector3(0.f, 0.f, 0.f), Graphics::LightType_Omni));
    // [Models]
    Graphics::RenderScene::Instance().Add("castle1", CModelHolder::s_pInstance->GetModelById("Castle OBJ.obj"));
    Graphics::RenderScene::Instance().Add("cyborg1", CModelHolder::s_pInstance->GetModelById("cyborg.obj"));
    Graphics::RenderScene::Instance().Add("ogre1", CModelHolder::s_pInstance->GetModelById("OgreOBJ.obj"));    

    Graphics::RenderScene::Instance().Add("bob", CModelHolder::s_pInstance->GetModelById("boblampclean.md5mesh"));
    Graphics::RenderScene::Instance().Scale("bob", IvVector3(0.05, 0.05, 0.05));
    Graphics::RenderScene::Instance().Rotate("bob", IvQuat(IvVector3(1, 0, 0), -90));
    // debug
    Graphics::RenderScene::Instance().Add("lightDebug", CModelHolder::s_pInstance->GetModelById("planet.obj"));
    Graphics::RenderScene::Instance().HasShadow("lightDebug", false);
    Graphics::RenderScene::Instance().Scale("lightDebug", IvVector3(0.1, 0.1, 0.1));
    Graphics::RenderScene::Instance().Scale("castle1", IvVector3(1.3, 1.0, 1.3));

    // 6 faces of the sky - external interface must provide these
    vector<std::string> faces;
    faces.push_back("skyright.bmp");
    faces.push_back("skyleft.bmp");
    faces.push_back("skybottom.bmp");
    faces.push_back("skytop.bmp");
    faces.push_back("skyfront.bmp");
    faces.push_back("skyback.bmp");
    Graphics::RenderScene::Instance().Add("SKY1", new UtilitiesCore::Skybox("sky1", faces));
    // update models location
    Graphics::RenderScene::Instance().Translate("bob", IvVector3(-4, 0.5f, 15));
    Graphics::RenderScene::Instance().Translate("ogre1", IvVector3(4, 0.5f, 15));
    Graphics::RenderScene::Instance().Translate("cyborg1", IvVector3(0, 0.5f, 15));


    Graphics::RenderScene::Instance().Rotate("ogre1", IvQuat(IvVector3(0, 1, 0), -15));
    Graphics::RenderScene::Instance().Rotate("cyborg1", IvQuat(IvVector3(0, 1, 0), 45));
    Graphics::RenderScene::Instance().Scale("SKY1", IvVector3(50, 50, 50));
    // [Landscape]

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

    /*Graphics::IRenderer::mRenderer->GetCamera().RotateX(0 * dt * 0.5f);
    Graphics::IRenderer::mRenderer->GetCamera().RotateY(5 * dt * 0.5f);*/

    // Update Debug objects
// [Light]
    static float m_lightAngle = 0.f;
    m_lightAngle += 0.515f;
    static float MoveRadius = 5.F;
    IvVector3 lightLocation(sin(m_lightAngle * 3.14159 / 180.F) * MoveRadius, 5.f, (cos(m_lightAngle * 3.14159 / 180.F) * MoveRadius) + 12.f);
    Graphics::Ilumination::Instance().Update("main", lightLocation);

    Graphics::RenderScene::Instance().Translate("lightDebug", lightLocation);
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

    // first pass, using the shadows depth shader
    Graphics::Ilumination::Instance().StartShadowsDepth();
    Graphics::RenderScene::Instance().Render(true);
    Graphics::Ilumination::Instance().FinishShadowsDepth();


    // adjust camera projection and view according to the current 
    // frustum parameters (3d - perspective mode)
    Graphics::IRenderer::mRenderer->PrepareCamera3D();
    // Render everything in the scene database
    Graphics::RenderScene::Instance().Render(false);
}

