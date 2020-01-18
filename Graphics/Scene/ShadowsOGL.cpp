#include "ShadowsOGL.h"
#include "CShaderHolder.h"
#include <GL/glew.h>
//#include "glm/glm.hpp"
#include "IRenderer.h"
#include "Ilumination.h"
#include <time.h>
#include "CommonMath.h"
#include "IvMath.h"

using namespace Graphics;

bool ShadowsOGL::Initialize()
{
	bool retVal = true;

	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &m_depthMapFBO);
	// create depth cubemap texture	
	glGenTextures(1, &m_depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// start loading measuring time
	clock_t start = clock();

	// read data from file
	const char shaderName[64] = "shadows";
	char vertexFilename[128], fragmentFilename[128], geometryFilename[128];
	sprintf(vertexFilename, "../Game/Assets/%sVertexshader.txt", shaderName);
	sprintf(fragmentFilename, "../Game/Assets/%sFragmentshader.txt", shaderName);
	sprintf(geometryFilename, "../Game/Assets/%sGeometryshader.txt", shaderName);
	// there should be a specialized class for this whole thing
	cwc::glShaderManager shaderLoader;
	m_pShader = shaderLoader.loadfromFile(vertexFilename, geometryFilename, fragmentFilename);

	if (m_pShader == 0)
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderName);
		retVal = false;
	}

	// time measurement
	printf(" loading shader [%s] %.2fms\n", shaderName, (float)(clock() - start));

	return retVal;
}

void Graphics::ShadowsOGL::Start(const IvVector3& lightPos)
{
	// 0. create depth cubemap transformation matrices
	// -----------------------------------------------
    // clear the depth buffer and the color buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    const Float near_plane = 0.1f;
	const Float far_plane = 25.f;

    // d is distance from view view position to the projection plane
    float d = 1.0f / IvTan(90.f / 180.0f * Types::s_PI * 0.5f);
    // a normalized 
    float recip = 1.0f / (near_plane - far_plane);
    IvMatrix44 perspective;

    perspective(0, 0) = d / ((GLfloat)1024 / (GLfloat)1024);
    perspective(1, 1) = d;
    perspective(2, 2) = (near_plane + far_plane)*recip;
    perspective(2, 3) = 2 * near_plane*far_plane*recip;
    perspective(3, 2) = -1.0f;
    perspective(3, 3) = 0.0f;

	//IvMatrix44 shadowProj = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
    IvMatrix44 lookAtMatrix;
	std::vector<IvMatrix44> shadowTransforms;
    CommonMath::LookAt(lightPos, lightPos + IvVector3(1.0f, 0.0f, 0.0f), IvVector3(0.0f, -1.0f, 0.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);
    CommonMath::LookAt(lightPos, lightPos + IvVector3(-1.0f, 0.0f, 0.0f), IvVector3(0.0f, -1.0f, 0.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);
    CommonMath::LookAt(lightPos, lightPos + IvVector3(0.0f, 1.0f, 0.0f), IvVector3(0.0f, 0.0f, 1.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);
    CommonMath::LookAt(lightPos, lightPos + IvVector3(0.0f, -1.0f, 0.0f), IvVector3(0.0f, 0.0f, -1.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);
    CommonMath::LookAt(lightPos, lightPos + IvVector3(0.0f, 0.0f, 1.0f), IvVector3(0.0f, -1.0f, 0.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);
    CommonMath::LookAt(lightPos, lightPos + IvVector3(0.0f, 0.0f, -1.0f), IvVector3(0.0f, -1.0f, 0.0f), lookAtMatrix);
	shadowTransforms.push_back(perspective * lookAtMatrix);

	// 1. render scene to depth cubemap
	// --------------------------------
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	// activate the Shadow Shade
	// get shader program id 
	GLuint programId = m_pShader->GetProgramObject();
	// enable it
	glUseProgram(programId);
	// check for errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Failed to apply shader ");
	
	for (UInt32 i = 0; i < 6; ++i)
	{
		string matrixName = "shadowMatrices[" + std::to_string(i) + "]";
		m_pShader->setUniformMatrix4fv(matrixName.c_str(), 1, false, (GLfloat*)shadowTransforms[i].GetFloatPtr());
		//simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	m_pShader->setUniform1f("far_plane", Graphics::IRenderer::mRenderer->GetFar());
	m_pShader->setUniform3f("lightPos", lightPos.GetX(), lightPos.GetY(), lightPos.GetZ());

    glErr = glGetError();
    if (glErr != 0)
        DEBUG_OUT("Failed Binding shadows");
}

void Graphics::ShadowsOGL::Stop()
{
    // unbind the depth map fbo 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // stop using shader
    glUseProgram(0);
}

UInt32 Graphics::ShadowsOGL::GetDepthMapId() const
{
	return m_depthCubemap;
}

void Graphics::ShadowsOGL::SetModel(IvMatrix44 & model)
{
    m_pShader->setUniformMatrix4fv("model", 1, false, (GLfloat*)model.GetFloatPtr());
}

void Graphics::ShadowsOGL::BindShadowTexture()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubemap);

    // check for errors
    Int32 glErr = glGetError();
    if (glErr != 0)
        DEBUG_OUT("Failed to bind Shadow Cube Texture ");
}
