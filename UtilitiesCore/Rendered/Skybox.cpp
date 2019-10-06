#include "Skybox.h"
// TODO: make this cross platform
#include "CModelOGL.h"
#include "glm/glm.hpp"

UtilitiesCore::Skybox::Skybox(const std::string& id, const vector<std::string>& faces)
{
	// allocate a open gl type model - can make this crossplatform using factories
	Graphics::CModelOGL* pSkyModel = new Graphics::CModelOGL(id);
	// allocates a temporary buffer to create the cube
	shared_ptr<Model> pModel = pSkyModel->Allocate();

	pSkyModel->SetShader("sky");

	// each face shall be an independently mesh

	// [TOP]
	Types::SModelMesh cubeTop = buildMesh(
		glm::vec3(-50.f,  50.f,  50.f),
		glm::vec3(-50.f,  50.f, -50.f),
		glm::vec3( 50.f,  50.f, -50.f),
		glm::vec3( 50.f,  50.f,  50.f), 
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[0]);

	// [BOTTOM]
	Types::SModelMesh cubeBottom = buildMesh(
		glm::vec3(-50.f, -50.f, -50.f),
		glm::vec3(-50.f, -50.f,  50.f),
		glm::vec3( 50.f, -50.f,  50.f),
		glm::vec3( 50.f, -50.f, -50.f),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[1]);

	// [LEFT]
	Types::SModelMesh cubeLeft = buildMesh(
		glm::vec3(-50.f, -50.f, -50.f),
		glm::vec3(-50.f,  50.f, -50.f),
		glm::vec3(-50.f,  50.f,  50.f),
		glm::vec3(-50.f, -50.f,  50.f),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[2]);

	// [RIGHT] .5f, .5f, .5f,   .5f,-.5f, .5f,   .5f,-.5f,-.5f,  .5f, .5f,-.5f
	Types::SModelMesh cubeRight = buildMesh(
		glm::vec3( 50.f, -50.f,  50.f),
		glm::vec3( 50.f,  50.f,  50.f),
		glm::vec3( 50.f,  50.f, -50.f),
		glm::vec3( 50.f, -50.f, -50.f),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[3]);

	// [BACK] .5f,-.5f,-.5f,  -.5f,-.5f,-.5f,  -.5f, .5f,-.5f,  .5f, .5f,-.5f
	Types::SModelMesh cubeBack = buildMesh(
		glm::vec3( 50.f, -50.f, -50.f),
		glm::vec3( 50.f,  50.f, -50.f),
		glm::vec3(-50.f,  50.f, -50.f),
		glm::vec3(-50.f, -50.f, -50.f),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[4]);

	// [FRONT] .5f, .5f, .5f,  -.5f, .5f, .5f,  -.5f,-.5f, .5f,  .5f,-.5f, .5f
	Types::SModelMesh cubeFront = buildMesh(
		glm::vec3(-50.f, -50.f,  50.f),
		glm::vec3(-50.f,  50.f,  50.f),
		glm::vec3( 50.f,  50.f,  50.f),
		glm::vec3( 50.f, -50.f,  50.f),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
		glm::vec2(1, 0),
		faces[5]);

	pModel->meshes.push_back(cubeTop);
	pModel->meshes.push_back(cubeBottom);
	pModel->meshes.push_back(cubeLeft);
	pModel->meshes.push_back(cubeRight);
	pModel->meshes.push_back(cubeBack);
	pModel->meshes.push_back(cubeFront);

	// actually create all the vbo, vba, textures and so on...
	pSkyModel->Commit();

	// finally, assign it to the sky model
	m_skyModel = pSkyModel;
	// resize this
	m_skyModel->SetScale(IvVector3(3, 3, 3));
}

UtilitiesCore::Skybox::~Skybox()
{
	if (m_skyModel != nullptr)
	{
		delete m_skyModel;
	}
}

void UtilitiesCore::Skybox::Draw()
{
	if (m_skyModel != nullptr)
	{
		m_skyModel->Draw();
	}
}

Types::SModelMesh UtilitiesCore::Skybox::buildMesh(
	const glm::vec3& v1,
	const glm::vec3& v2,
	const glm::vec3& v3, 
	const glm::vec3& v4, 
	const glm::vec2& uv1,
	const glm::vec2& uv2,
	const glm::vec2& uv3,
	const glm::vec2& uv4,
	const std::string& texture)
{
	// return value
	Types::SModelMesh meshValue;
	// straight forward
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(1);
	meshValue.m_indices.push_back(2);
	meshValue.m_indices.push_back(0);
	meshValue.m_indices.push_back(3);
	meshValue.m_indices.push_back(2);

	// normal vector
	glm::vec3 nm(0.0f, 1.0f, 0.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = v2 - v1;
	glm::vec3 edge2 = v3 - v1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// vertexes
	SModelVertex vertices;
	vertices.Position = v1;
	vertices.Bitangent = bitangent1;
	vertices.Tangent = tangent1;
	vertices.Normal = nm;
	vertices.TexCoords = uv1;
	meshValue.m_vertices.push_back(vertices);
	vertices.Position = v2;
	vertices.TexCoords = uv2;
	meshValue.m_vertices.push_back(vertices);

	// triangle 2
	// ----------
	edge1 = v3 - v1;
	edge2 = v4 - v1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);

	vertices.Position = v3;
	vertices.Bitangent = bitangent2;
	vertices.Tangent = tangent2;
	vertices.Normal = nm;
	vertices.TexCoords = uv3;
	meshValue.m_vertices.push_back(vertices);
	vertices.Position = v4;
	vertices.TexCoords = uv4;
	meshValue.m_vertices.push_back(vertices);

	// texture
	SModelTexture tex;
	tex.m_filename = texture;
	tex.m_uniformName = "diffuseMap";
	meshValue.m_textures.push_back(tex);

	return meshValue;

}
