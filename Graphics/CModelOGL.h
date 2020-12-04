#ifndef _CMODELOGL_H_
#define _CMODELOGL_H_

#include "CommonTypes.h"
#include "ogltypes.h"
#include "IModel.h"
#include "Shaders\glsl.h"

using namespace Types;

namespace Graphics
{

	class CModelOGL : public IModel
	{
	public:
		CModelOGL(string id);
		virtual ~CModelOGL();

		static const UInt32 MAX_BONES = 100;
        
        // create based on SModelData
        virtual bool Create() override;
		virtual bool Apply(const Model& modelInfo) override;
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
		GLuint m_bone_location[MAX_BONES];
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

	};


}
#endif // _CMODELOGL_H_