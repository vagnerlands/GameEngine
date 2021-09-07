#include "CParticle.h"
#include <iostream>
#include "SceneItemParticlesSystem.h"
#include "MutexFactory.h"
#include <time.h>

Graphics::CParticle::CParticle(string particleName) :
	m_pShader(nullptr)
{
	// only for debug purposes
	m_particleName = particleName;
}

Graphics::CParticle::~CParticle()
{

	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

	if (m_pShader != nullptr)
	{
		delete m_pShader;
		m_pShader = nullptr;
	}
}

void Graphics::CParticle::Create()
{
	// The VBO containing the 4 vertices of the particles.
	static Float boxVertexes[]  = {
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f,
	  0.5f,  0.5f, 0.0f,
};

	//m_indicesCount = sizeof(indices) / sizeof(indices[0]);

	// first, try to compile the shader
	m_pShader = generateShader("particles");


	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertexes), &boxVertexes[0], GL_DYNAMIC_DRAW);

	// element buffer object allocation
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// check for GL errors
	Int32 error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}
}

void Graphics::CParticle::Update(float dt)
{

}

void Graphics::CParticle::Draw(const SceneItem& si, float dt, bool isRenderingShadows)
{
	if (isRenderingShadows)
		return;
	// clear any non-treated Gl errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Untreated error has been found");

	// breaking a princicle here
	const SceneItemParticlesSystem& pParticleSceneInstance = dynamic_cast<const SceneItemParticlesSystem&>(si);

	si.SetUpScene(m_pShader);

	// draw all meshes mesh
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	for (Int32 i = 0; i < pParticleSceneInstance.GetNumberOfParticles(); ++i)
	{
		pParticleSceneInstance.SetUpParticleAttributes(m_pShader, i);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	glDisable(GL_BLEND);
	// check for GL errors
	Int32 error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error: " << error << std::endl;
	}
	// always good practice to set everything back to defaults once configured.
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
}

const AABB & Graphics::CParticle::GetBoundaryBox() const
{
    static const AABB aabb;
    return aabb;
}

#pragma optimize( "", off )
cwc::glShader* Graphics::CParticle::generateShader(const string& shaderName)
{
	// return value
	bool status = true;
	// start loading measuring time
	clock_t start = clock();

	// read data from file
	cwc::glShader* pShader = nullptr;
    const UInt32 cMaxFilenameLen = 128U;
    char vertexFilename[cMaxFilenameLen] = { 0 };
    char fragmentFilename[cMaxFilenameLen] = { 0 };
	sprintf_s(vertexFilename, cMaxFilenameLen, "./Assets/%sVertexshader.txt", shaderName.data());
	sprintf_s(fragmentFilename, cMaxFilenameLen, "./Assets/%sFragmentshader.txt", shaderName.data());
	// there should be a specialized class for this whole thing
	cwc::glShaderManager shaderLoader;

	bool isGeometryFileNotFound = false;

	// checks if the geometry shader actually is required
	pShader = shaderLoader.loadfromFile(vertexFilename, fragmentFilename);

	if (pShader == nullptr)
	{
		printf("<!> Failed to parse shader files [%s]\n", shaderName.data());
		status = false;
	}

	// time measurement
	printf(" [*] loading shader [%s] %.2fms\n", shaderName.data(), (float)(clock() - start));

	return pShader;
}
#pragma optimize( "", on )

