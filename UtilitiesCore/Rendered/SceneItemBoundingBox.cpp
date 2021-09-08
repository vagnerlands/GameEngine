#include "SceneItemBoundingBox.h"
#include "Factory/ModelFactory.h" 
#include "OpenGL/CModelOGL.h"
//#include "glm/glm.hpp"

namespace UtilitiesCore
{
    shared_ptr<SceneItemBoundingBox> SceneItemBoundingBox::CreateBoundingBox(const Graphics::SceneItem& mesh)
    {
        return std::make_shared<SceneItemBoundingBox>(mesh);
    }

    SceneItemBoundingBox::SceneItemBoundingBox(const Graphics::SceneItem & mesh) : 
        Graphics::SceneItem("bb", nullptr),
        mMeshReference(mesh),
        m_sceneItem("bb", shared_ptr<Graphics::IDrawable>(nullptr))
    {
        // allocate a open gl type model - can make this crossplatform using factories
        auto pBoundingBoxModel = Graphics::ModelFactory::Instance().Create("bb");
        // allocates a temporary buffer to create the cube
        shared_ptr<Model> pModel = pBoundingBoxModel->Allocate();

        Types::SModelMesh meshValue;        
        meshValue.m_shaderName = "boundingBox";
        // box vertexes
        vector<IvVector3> vertices = {
            IvVector3(-1.0f, -1.0f, -1.0f),
            IvVector3(1.0f, -1.0f, -1.0f),
            IvVector3(1.0f, 1.0f, -1.0f),
            IvVector3(-1.0f, 1.0f, -1.0f),
            IvVector3(-1.0f, -1.0f, 1.0f),
            IvVector3(1.0f, -1.0f, 1.0f),
            IvVector3(1.0f, 1.0f, 1.0f),
            IvVector3(-1.0f, 1.0f, 1.0f)
        };


        // try to cast to CModelOGL type
        const Graphics::CModelOGL* modelOglRef = dynamic_cast<const Graphics::CModelOGL*>(mMeshReference.GetDrawable().get());
        if (modelOglRef != nullptr)
        {
            const AABB& bb = modelOglRef->GetBoundaryBox();
            vertices.clear();
            vertices = {
                IvVector3(bb.Min.GetX(), bb.Min.GetY(),bb.Min.GetZ()),
                IvVector3(bb.Max.GetX(), bb.Min.GetY(),bb.Min.GetZ()),
                IvVector3(bb.Max.GetX(), bb.Max.GetY(),bb.Min.GetZ()),
                IvVector3(bb.Min.GetX(), bb.Max.GetY(),bb.Min.GetZ()),
                IvVector3(bb.Min.GetX(), bb.Min.GetY(),bb.Max.GetZ()),
                IvVector3(bb.Max.GetX(), bb.Min.GetY(),bb.Max.GetZ()),
                IvVector3(bb.Max.GetX(), bb.Max.GetY(),bb.Max.GetZ()),
                IvVector3(bb.Min.GetX(), bb.Max.GetY(),bb.Max.GetZ())
            };
        }

        const Int32 cCountElements = vertices.size();

        for (Int32 i = 0; i < cCountElements; ++i)
        {
            SModelVertex entry;
            entry.Position = vertices[i];
            entry.TexCoords = IvVector2(0.f,0.f);
            meshValue.m_vertices.push_back(entry);
        }

        meshValue.m_indices = {
            5, 4, 0, 1, 5, 0, 6, 5, 1, 2, 6, 1,
            7, 6, 2, 3, 7, 2, 4, 7, 3, 0, 4, 3,
            6, 7, 4, 5, 6, 4, 1, 0, 3, 2, 1, 3 
        };


        // actually create all the vbo, vba, textures and so on...
        pModel->meshes.push_back(meshValue);        
        pBoundingBoxModel->Commit();
        /*this*/ReplaceDrawable(pBoundingBoxModel);
        /*this*/SetCastShadows(false);
        pBoundingBoxModel->SetWireMode(true);
    }

    SceneItemBoundingBox::~SceneItemBoundingBox()
    {
        m_sceneItem.Release();
    }
}