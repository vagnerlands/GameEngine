#include "Skybox.h"
// TODO: make this cross platform
#include "CModelOGL.h"
#include "glm/glm.hpp"

UtilitiesCore::Skybox::Skybox(const std::string& id, const vector<std::string>& faces) : 
	Graphics::IDrawable(), 
	m_skyModel(id, this)
{
	// constant normalized box size - shall be used for texture mapping too
	const Float cBoxSize = 1.f;
	// allocate a open gl type model - can make this crossplatform using factories
	Graphics::CModelOGL* pSkyModel = new Graphics::CModelOGL(id);
	// allocates a temporary buffer to create the cube
	shared_ptr<Model> pModel = pSkyModel->Allocate();

	// mesh
	Types::SModelMesh meshValue;
	meshValue.m_shaderName = "sky";
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(2);
	meshValue.m_indices.push_back(2);
	meshValue.m_indices.push_back(3);
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(5);
	meshValue.m_indices.push_back(6);
	meshValue.m_indices.push_back(6);
	meshValue.m_indices.push_back(2);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(7);
	meshValue.m_indices.push_back(6);
	meshValue.m_indices.push_back(5);
	meshValue.m_indices.push_back(5);
	meshValue.m_indices.push_back(4);
	meshValue.m_indices.push_back(7);
	meshValue.m_indices.push_back(4);
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(3);
	meshValue.m_indices.push_back(3);
	meshValue.m_indices.push_back(7);
	meshValue.m_indices.push_back(4);
	meshValue.m_indices.push_back(4);
	meshValue.m_indices.push_back(5);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(4);
	meshValue.m_indices.push_back(3);
	meshValue.m_indices.push_back(2);
	meshValue.m_indices.push_back(6);
	meshValue.m_indices.push_back(6);
	meshValue.m_indices.push_back(7);
	meshValue.m_indices.push_back(3);


	// texture
	SModelTexture tex;
	//tex.m_filename = texture;
	tex.m_uniformName = "diffuseMap";
	tex.m_isCubeMap = true;
	meshValue.m_textures.push_back(tex);

	glm::vec3 boxVertexes[] =
	{
		glm::vec3(-1.0, -1.0,  1.0),
		glm::vec3(1.0, -1.0,  1.0),
		glm::vec3(1.0,  1.0,  1.0),
		glm::vec3(-1.0,  1.0,  1.0),
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0,  1.0, -1.0),
		glm::vec3(-1.0,  1.0, -1.0)
	};

	glm::vec2 boxTextCoords[] = 
	{
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0)
	};

	const Int32 cCountElements = sizeof(boxVertexes) / sizeof(boxVertexes[0]);

	for (Int32 i = 0; i < cCountElements; ++i)
	{
		SModelVertex vertices;
		vertices.Position = boxVertexes[i];
		vertices.TexCoords = boxTextCoords[i%4];
		meshValue.m_vertices.push_back(vertices);
	}

	// make sure the vector is clear
	meshValue.m_textures.clear();
	// adds all faces to the mesh
	for (Int32 i = 0; i < faces.size(); ++i)
	{ 
		// creates a model texture to be used - marks this as a cubemap for texture generation purposes
		SModelTexture textAttr;
		textAttr.m_filename = faces[i];
		textAttr.m_uniformName = "diffuseMap";
		textAttr.m_isCubeMap = true;
		// adds all faces to the mesh
		meshValue.m_textures.push_back(textAttr);
	}

	pModel->meshes.push_back(meshValue);

	// actually create all the vbo, vba, textures and so on...
	pSkyModel->Commit();

	// finally, assign it to the sky model
	m_skyModel.ReplaceDrawable(pSkyModel);
    m_skyModel.SetCastShadows(false);
	// resize this
	m_skyModel.SetScale(IvVector3(150, 150, 150));
}

UtilitiesCore::Skybox::~Skybox()
{
	m_skyModel.Release();
}

void UtilitiesCore::Skybox::Update(float dt)
{
	// perhaps if we wanted to make some real time effects on the sky, here is the place for this...
}

void UtilitiesCore::Skybox::Draw(const Graphics::SceneItem& , float dt, bool isRenderingShadows)
{
	// in case we're currently rendering shadows, the skybox shouldn't affect it
	if (isRenderingShadows) return;
	// otherwise, proceed regularly
	m_skyModel.Render(dt, isRenderingShadows);
}

void UtilitiesCore::Skybox::Tick(float delta_time)
{
	// TODO: maybe animate the sky by playing a bit with the textures
}
