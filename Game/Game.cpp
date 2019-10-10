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
	Graphics::IRenderer::mRenderer->SetFOV(90.0F);
	// Update camera to be above ground
	Graphics::IRenderer::mRenderer->GetCamera().m_position.SetY(1.f);
	// Update this camera type
	Graphics::IRenderer::mRenderer->GetCamera().m_type = Camera_Spectator;
	// create model holder
	CModelHolder::s_pInstance->Create("..\\Game\\Assets\\model.zip");
    // 50mb allocation for VRAM textures
	CTextureHolder::s_pInstance->Create("..\\Game\\Assets\\textures.zip", 200U*1024U*1024U);

	IGame::mGame->SetFps(60);

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

	// [Model]

	// [Landscape]

}

void Game::Render()
{
	// prepares MODEL VIEW
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	// Set background (clear) color to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	// adjust camera projection and view according to the current 
	// frustum parameters (3d - perspective mode)
	Graphics::IRenderer::mRenderer->PrepareCamera3D();
	
	// Render everything in the scene database
	Graphics::RenderScene::Instance().Render();


 //   glPushMatrix();
 //   CShaderHolder::s_pInstance->UseShaderById("texturedtangent");

 //   model = glm::identity<glm::mat4>();
	//glEnable(GL_TEXTURE_2D);
	//CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->setTexture("diffuseMap", 
	//	CTextureHolder::s_pInstance->getTextureById("wallstone-diffuse.bmp"));

	//CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->setTexture("normalMap",
	//	CTextureHolder::s_pInstance->getTextureById("wallstone-normal.bmp"));

	//CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->setTexture("specularMap",
	//	CTextureHolder::s_pInstance->getTextureById("wallstone-specular.bmp"));

 //   CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->
 //       setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
 //   CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->
 //       setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());

 //   CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->setUniform3f("lightPos", light_position[0], light_position[1], light_position[2]);

 //   CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->
 //       setUniformMatrix4fv("model", 1, false, &model[0][0]);

 //   CShaderHolder::s_pInstance->GetShaderProgramById("texturedtangent")->setUniform3f("viewPos", 
 //       Graphics::IRenderer::mRenderer->GetCamera().m_position.GetX(),
 //       Graphics::IRenderer::mRenderer->GetCamera().m_position.GetY(),
 //       Graphics::IRenderer::mRenderer->GetCamera().m_position.GetZ());

 //   static UInt32 quadVAO = 0;
 //   static UInt32 quadVBO = 0;

 //   if (quadVAO == 0)
 //   {
 //       // positions
 //       glm::vec3 pos1(-10.0f, -2.0f,  10.0f);
 //       glm::vec3 pos2(-10.0f, -2.0f, -10.0f);
 //       glm::vec3 pos3( 10.0f, -2.0f, -10.0f);
 //       glm::vec3 pos4( 10.0f, -2.0f,  10.0f);
 //       // texture coordinates
 //       glm::vec2 uv1(0.0f, 1.0f);
 //       glm::vec2 uv2(0.0f, 0.0f);
 //       glm::vec2 uv3(1.0f, 0.0f);
 //       glm::vec2 uv4(1.0f, 1.0f);
 //       // normal vector
 //       glm::vec3 nm(0.0f, 1.0f, 0.0f);

 //       // calculate tangent/bitangent vectors of both triangles
 //       glm::vec3 tangent1, bitangent1;
 //       glm::vec3 tangent2, bitangent2;
 //       // triangle 1
 //       // ----------
 //       glm::vec3 edge1 = pos2 - pos1;
 //       glm::vec3 edge2 = pos3 - pos1;
 //       glm::vec2 deltaUV1 = uv2 - uv1;
 //       glm::vec2 deltaUV2 = uv3 - uv1;

 //       GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

 //       tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
 //       tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
 //       tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
 //       tangent1 = glm::normalize(tangent1);

 //       bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
 //       bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
 //       bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
 //       bitangent1 = glm::normalize(bitangent1);

 //       // triangle 2
 //       // ----------
 //       edge1 = pos3 - pos1;
 //       edge2 = pos4 - pos1;
 //       deltaUV1 = uv3 - uv1;
 //       deltaUV2 = uv4 - uv1;

 //       f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

 //       tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
 //       tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
 //       tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
 //       tangent2 = glm::normalize(tangent2);


 //       bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
 //       bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
 //       bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
 //       bitangent2 = glm::normalize(bitangent2);


 //       float quadVertices[] = {
 //           // positions            // normal         // texcoords  // tangent                          // bitangent
 //           pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
 //           pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
 //           pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

 //           pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
 //           pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
 //           pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
 //       };
 //       // configure plane VAO
 //       glGenVertexArrays(1, &quadVAO);
 //       glGenBuffers(1, &quadVBO);
 //       glBindVertexArray(quadVAO);
 //       glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
 //       glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
 //       glEnableVertexAttribArray(0);
 //       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
 //       glEnableVertexAttribArray(1);
 //       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
 //       glEnableVertexAttribArray(2);
 //       glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
 //       glEnableVertexAttribArray(3);
 //       glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
 //       glEnableVertexAttribArray(4);
 //       glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
 //   }
 //   glBindVertexArray(quadVAO);
 //   //glDrawArrays(GL_TRIANGLES, 0, 6);
 //   glBindVertexArray(0);
	//CShaderHolder::s_pInstance->StopShader();
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glActiveTexture(GL_TEXTURE0);
	//glDisable(GL_LIGHTING);
 //   glPopMatrix();

	//// [ TEXTURED SQUARE ]
	//glPushMatrix();
	//glEnable(GL_TEXTURE_2D);	
	//CTextureHolder::s_pInstance->Bind("brick_t.bmp");
 //   glColor4f(1, 0, 1, 1);
	//glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5f);
	//glBegin(GL_QUADS);
	//glVertex3f(-200, -100, -1000); glTexCoord2f(0, 0);
	//glVertex3f(100, -100, -1000); glTexCoord2f(0, 1);
	//glVertex3f(100, 100, -1000); glTexCoord2f(1, 1);
	//glVertex3f(-200, 100, -1000); glTexCoord2f(1, 0);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_BLEND);
	//glDisable(GL_ALPHA_TEST);
	//glPopMatrix();


	//// [ ROTATING TRIANGLE ]
	//glPushMatrix();
 //   static Int32 inc = 0;
	//glRotatef(160.f + ++inc, 0, 0, 1);
	//glBegin(GL_TRIANGLES);
	//glVertex3f(0, 0, -1000);
	//glVertex3f(100, 50, -1000);
	//glVertex3f(640 / 2, 0, -1000);
	//glEnd();
	//glPopMatrix();
	//// [TEXTURED GROUND]
	//

	glutSwapBuffers();
}

