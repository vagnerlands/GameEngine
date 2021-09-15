
#include "IGame.h"
#include "IRenderer.h"
#include "IClock.h"
#include "CClockOGL.h"
#include "CThreadHolder.h"
//#include "GL/glut.h"
#include "RenderUI.h"
#include "Ilumination.h"
#include "RenderScene.h"
#include "CModelHolder.h"

#include "CShaderHolder.h"
#include "CModelHolder.h"
#include "CParticlesSystemHolder.h"
#include "CSoundHolder.h"
#include "CTextureHolder.h"
#include "Utils/Jobs.h"

#include "OpenGL/Window2DOGL.h"
#include "Logger/ILogger.h"

EngineCore::IGame* EngineCore::IGame::mGame = nullptr;

void EngineCore::IGame::Destroy()
{
    if (mGame->mClock != nullptr)
        mGame->mClock = nullptr;
    if (mGame != nullptr)
		delete mGame;	
}

bool EngineCore::IGame::PreRendererInitialize(int argc, char * argv[])
{
	if (!ParseCommandLine(argc, argv))
	{ }
		

	if (!SetupSubsystems(Types::EGraphicsAPI::EGraphics_OGL))
	{ }

    //TODO: initialize lighting default values (some basic light somewhere)
    CShaderHolder::Create();
    // create model holder
    CModelHolder::s_pInstance->Create(".\\Assets\\model.zip");
    CParticlesSystemHolder::s_pInstance->Create(".\\Assets\\model.zip");
    CSoundHolder::s_pInstance->Create("test", 1);
    // allocation for VRAM textures
    const UInt32 cTextureCacheSizeInMBs = 120;
    CTextureHolder::s_pInstance->Create(".\\Assets\\textures.zip", cTextureCacheSizeInMBs * 1024U * 1024U);

	return true;
}

bool EngineCore::IGame::PostRendererInitialize() 
{

	return true;
}

void EngineCore::IGame::ExecuteBackground()
{
	// this method should be overloaded
    _Utils::Jobs::Instance().Process();
}


void EngineCore::IGame::Display()
{
	// checks whether the application is still running
	if (mGame->IsRunning())
	{
		// execute events and update databases
		mClock->StartFrame();
		// update all objects - the delta time (elapsed time)
		// is a good feature to increase intensity of movement 
		// if a few frames were lost - making the movements flow
		UpdateObjects(mClock->GetElapsedTime());
		// render a scene according to current position of objects
		Render(mClock->GetTimeInMili());

        // build textures, if there are any to be build...
        CTextureHolder::s_pInstance->Update();

		mClock->Hold(mGame->GetMaxFPS());
	}

	if (ReadyToClose())
	{
		gDebugger.Flush();
		// kill all background threads
		CThreadHolder::instance()->DestroyAll();
		// release the rendering resources
		Graphics::IRenderer::Destroy();
        UtilitiesCore::ILogger::Destroy();
        Graphics::RenderUI::Instance().Destroy();
		// release other game related resources
		EngineCore::IGame::Destroy();        
		// exit with no error if get to this point
		exit(0);
	}

}

EngineCore::IGame::IGame() :
	mPaused(false), 
	mQuit(false),
	mReadyToClose(false),
	mMaxFps(0),
	mClosedThreads(0),
    mGameController(1280, 720)
{
    KeyDispatcherFactory::Create(&mGameController);    

    auto logger = Graphics::RenderUI::Instance().Add("logger", std::make_shared<Graphics::Window2DOGL>(IvVector4(0.f, 0.f, 0.f, .5f), ""));
    logger->SetLocation({ 20.f, 40.f }).SetSize({ 400.f, 480.f });    

    mpLogger = UtilitiesCore::ILogger::Create(logger);

    auto logger_input = Graphics::RenderUI::Instance().Add("logger_input", std::make_shared<Graphics::Window2DOGL>(IvVector4(1.f, 1.f, 1.f, 1.f),""));
    logger_input->SetLocation({ 0.f, -20.f }).SetSize({ 400.f, 20.f });
    logger->Append(logger_input);    
}

EngineCore::IGame::~IGame()
{
    // release all allocated resources
    delete CParticlesSystemHolder::s_pInstance;
    delete CTextureHolder::s_pInstance;
    delete CShaderHolder::s_pInstance;
    delete CModelHolder::s_pInstance;
}

bool EngineCore::IGame::ParseCommandLine(int argc, char * argv[])
{
	//TODO: parse the input commands
	return true;
}

bool EngineCore::IGame::SetupSubsystems(Types::EGraphicsAPI gfxApi)
{
	// initialize clock
	if (gfxApi == Types::EGraphicsAPI::EGraphics_OGL)
	{
		// Open GL implementation
		mClock = std::make_shared<UtilitiesCore::CClockOGL>();
	}
	else
	{
		// no implementation found - recommend to quit this application in this case
		return false;
	}
	// TODO: initialize mutexes/semaphores

	return true;
}

void 
EngineCore::IGame::Render(float dt)
{   
    CModelHolder::s_pInstance->Refresh(); // load pending objects into the Model database
    
    Graphics::IRenderer::mRenderer->PrepareFrame(); // set which buffers are used, set the modelview matrix and more

    // first pass, using the shadows depth shader
    Graphics::Ilumination::Instance().StartShadowsDepth();
    Graphics::RenderScene::Instance().Render(dt, true);
    Graphics::Ilumination::Instance().FinishShadowsDepth();

    // adjust camera projection and view according to the current 
    // frustum parameters (3d - perspective mode)
    Graphics::IRenderer::mRenderer->PrepareCamera3D();    
    Graphics::RenderScene::Instance().Render(dt, false); // Render everything in the scene database

    // adjust camera projection and view according to the current 
    // ortho parameters (2d - orthogonal mode)
    Graphics::IRenderer::mRenderer->PrepareCamera2D();
    Graphics::RenderUI::Instance().Render(dt); // render all UI objects
}

void
EngineCore::IGame::PostRender()
{
    Graphics::IRenderer::mRenderer->SwapBuffer();
}

void
EngineCore::IGame::Reshape(Int32 w, Int32 h)
{
	Graphics::IRenderer::mRenderer->Resize(w, h);
    mGameController.Resize(w, h);
}

Int32 EngineCore::IGame::GetFPS() const
{
    return mClock->FramesPerSecond();
}