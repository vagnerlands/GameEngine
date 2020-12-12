#ifndef _CMODELOGL_H_
#define _CMODELOGL_H_

#include "CommonTypes.h"
#include "ogltypes.h"
#include "IModel.h"
#include "Shaders\glsl.h"

#include "IvMatrix44.h"

using namespace Types;

namespace Graphics
{

	class CModelOGL : public IModel
	{
	public:
		CModelOGL(string id);
		virtual ~CModelOGL();
        
        // create based on SModelData
        virtual bool Create() override;
		virtual bool Apply(const Model& modelInfo) override;
		// periodic call to update
		virtual void Update(float dt) override;
		//bool SetShader(const string& shaderName);
		virtual void Draw(float dt, bool isRenderingShadows);
		// allocate SModelData for custom objects
		virtual shared_ptr<Model> Allocate();
		// commit changes
		virtual bool Commit();

	protected:

		cwc::glShader* generateShader(const string& shaderName);

        struct SDrawData
        {
            SDrawData() : m_vertexArrayObject(0U), m_indicesCount(0U) {}
            explicit SDrawData(UInt32 vao, UInt32 indCount, vector<SModelTexture> vTextures, cwc::glShader* pShader) : 
				m_vertexArrayObject(vao), 
				m_indicesCount(indCount), 
				m_textures(vTextures),
				m_pShader(pShader)
			{
				// empty
			}
       
			vector<SModelTexture>   m_textures;
            UInt32					m_vertexArrayObject;
            UInt32					m_indicesCount;

			cwc::glShader*			m_pShader;
        };

		// copy operations
		CModelOGL(const CModelOGL& other) = delete;
		CModelOGL& operator=(const CModelOGL& other) = delete;

		bool                    m_vboBufferCreated;
		// allocated resources, to be released on destructor
        vector<UInt32>          m_vertexBufferObject;
		// allocated resources, to be released on destructor
		vector<UInt32>          m_bonesBufferObject;
		// allocated resources, to be released on destructor
        vector<UInt32>          m_elementBufferObject;
        vector<SDrawData>       m_drawAttr;   
		// bones (if applicable)
		UInt32 m_bone_location[MAX_BONES];
		float ticks_per_second = 0.0f;
		aiMatrix4x4 m_global_inverse_transform;
		// assimp importer, keep a reference for the scene
		Assimp::Importer		m_Importer;
		bool m_hasAnimations;
		bool m_isWireMode;

	private:
		UInt32 findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		UInt32 findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		UInt32 findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
		const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const string p_node_name);
		// calculate transform matrix
		aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
		glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
		aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend); // super super n lerp =)
		void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
		void boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms);
		SBoneInformation m_boneInformation;
		// placeholder for animations purposes
		vector<aiMatrix4x4> m_boneTransforms;
		IvMatrix44 castToIvMatrix44(const aiMatrix4x4& input) const;
	};


	inline IvMatrix44 CModelOGL::castToIvMatrix44(const aiMatrix4x4& input) const
	{
		IvMatrix44 ret;

		ret.SetRows(IvVector4(m_Importer.GetScene()->mRootNode->mTransformation.a1, m_Importer.GetScene()->mRootNode->mTransformation.a2, m_Importer.GetScene()->mRootNode->mTransformation.a3, m_Importer.GetScene()->mRootNode->mTransformation.a4),
			IvVector4(m_Importer.GetScene()->mRootNode->mTransformation.b1, m_Importer.GetScene()->mRootNode->mTransformation.b2, m_Importer.GetScene()->mRootNode->mTransformation.b3, m_Importer.GetScene()->mRootNode->mTransformation.b4),
			IvVector4(m_Importer.GetScene()->mRootNode->mTransformation.c1, m_Importer.GetScene()->mRootNode->mTransformation.c2, m_Importer.GetScene()->mRootNode->mTransformation.c3, m_Importer.GetScene()->mRootNode->mTransformation.c4),
			IvVector4(m_Importer.GetScene()->mRootNode->mTransformation.d1, m_Importer.GetScene()->mRootNode->mTransformation.d2, m_Importer.GetScene()->mRootNode->mTransformation.d3, m_Importer.GetScene()->mRootNode->mTransformation.d4));

		return ret;
	}

}
#endif // _CMODELOGL_H_