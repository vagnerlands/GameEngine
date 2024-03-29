#include "SceneItem.h"
#include "IvMatrix44.h"
#include "IRenderer.h"
#include "Ilumination.h"
#include "Rendered/SceneItemBoundingBox.h"

Graphics::SceneItem::SceneItem(const std::string& id, shared_ptr<Graphics::IDrawable> pDrawable) :
	m_pDrawable(pDrawable),
	m_sceneItemId(id),
	m_location(0.f, 0.f, 0.f), // default location
	m_scale(1.f, 1.f, 1.f), // default scale (original size from editor)
	m_uvFactor(1.f, 1.f),
	m_hasShadow(true),
    m_primitive(eRenderingPrimitive_Triangles)
{
    if (pDrawable != nullptr)
        m_pBoundingBox = pDrawable->GetBoundaryBox();
	m_rotation.Identity();
}

void Graphics::SceneItem::Render(float dt, bool isRenderingShadows) const
{
    if (m_pDrawable != nullptr)
    {
        m_pDrawable->Draw(*this, dt, isRenderingShadows);
        if (m_nextScene != nullptr)
        {
            m_nextScene->Render(dt, isRenderingShadows);
        }
    }
}

void Graphics::SceneItem::ReplaceDrawable(shared_ptr<Graphics::IDrawable> pDrawable)
{
	m_pDrawable = pDrawable;
    m_pBoundingBox = pDrawable->GetBoundaryBox();
}

void Graphics::SceneItem::Release()
{
    // release 1 shared pointer counter
    m_pDrawable = nullptr;
}

void Graphics::SceneItem::updateModel()
{
	// translate model
	IvMatrix44 translateModel;
	translateModel.Identity();
	// rotate model
	IvMatrix44 rotateModel;
	rotateModel.Identity();
	// scale model
	IvMatrix44 scaleModel;
	scaleModel.Identity();
	// calculates idependently each transformation
	// rotate transformation
	rotateModel.Rotation(GetRotation());
	// scale transformation
	scaleModel.Scaling(GetScale());
	// translation transformation
	translateModel.Translation(GetLocation());
	// combine both transformations
	m_model = translateModel * scaleModel * rotateModel;
}

void Graphics::SceneItem::SetUpScene(cwc::glShader* pShader) const
{
	// get shader program id 
	pShader->Use();
	// check for errors
	Int32 glErr = glGetError();
	if (glErr != 0)
		DEBUG_OUT("Failed to apply shader ");

	// Apply attributes known for this shader
	IvMatrix44& projMatrix = Graphics::IRenderer::mRenderer->GetProjectionMatrix();
	IvMatrix44& viewMatrix = Graphics::IRenderer::mRenderer->GetViewMatrix();

	Float lightLocation[4] = { 0 };
	Graphics::Ilumination::Instance().GetIluminationItemLocationPtr("main", lightLocation);

	pShader->setUniformMatrix4fv("projection", 1, false, (GLfloat*)projMatrix.GetFloatPtr());
	pShader->setUniformMatrix4fv("view", 1, false, (GLfloat*)viewMatrix.GetFloatPtr());
	if (HasShadows())
	{
		pShader->setUniform3f("viewPos",
			Graphics::IRenderer::mRenderer->GetCamera().m_position.GetX(),
			Graphics::IRenderer::mRenderer->GetCamera().m_position.GetY(),
			Graphics::IRenderer::mRenderer->GetCamera().m_position.GetZ());
		pShader->setUniform3f("lightPos", lightLocation[0], lightLocation[1], lightLocation[2]);
		const IvVector3& lc = Graphics::Ilumination::Instance().GetLightColor();
		pShader->setUniform3f("light_color", lc.GetX(), lc.GetY(), lc.GetZ());
		const IvVector3& ac = Graphics::Ilumination::Instance().GetAmbientLightColor();
		pShader->setUniform3f("ambient_color", ac.GetX(), ac.GetY(), ac.GetZ());
		// update the boolean flag for "has shadows"
		pShader->setUniform1i("cast_shadows", HasShadows());
		pShader->setUniform1f("light_attenuation", Graphics::Ilumination::Instance().GetLightAttenuation());
        pShader->setUniform1f("bias", Graphics::IRenderer::mRenderer->GetBias());
        pShader->setUniform1f("shadow_factor", Graphics::IRenderer::mRenderer->GetShadowFactor());
		pShader->setUniform1f("far_plane", Graphics::IRenderer::mRenderer->GetFar());
		pShader->setUniform1i("depthMap", 2);
	}
	
	pShader->setUniformMatrix4fv("model", 1, false, (GLfloat*)m_model.GetFloatPtr());

}

void Graphics::SceneItem::SetUpAnimation(cwc::glShader* pShader) const 
{

}

void Graphics::SceneItem::SetUpShadows(cwc::glShader* pShader)  const 
{
	// no shadowing, do nothing
}

void Graphics::SceneItem::ShadowsPass() const
{
	Graphics::Ilumination::Instance().HasAnimations(0);
	Graphics::Ilumination::Instance().UpdateModel(m_model);
}

Graphics::SceneItem& 
Graphics::SceneItem::DisplayBoundingBox(bool display)
{
    m_nextScene = (display) ? (UtilitiesCore::SceneItemBoundingBox::CreateBoundingBox(*this)) : nullptr;
    if (m_nextScene != nullptr)
    {
        m_nextScene->SetLocation(GetLocation());
        m_nextScene->SetRotation(GetRotation());
        m_nextScene->SetScale(GetScale());
    }
    return *this;
}

Graphics::SceneItem & 
Graphics::SceneItem::SetRenderingPrimitive(eRenderingPrimitive primitive)
{
    m_primitive = primitive;
    return *this;
}

IvAABB
Graphics::SceneItem::GetBoundingBox() const
{
    return IvAABB(m_model.Transform(m_pBoundingBox->GetMinima()), m_model.Transform(m_pBoundingBox->GetMaxima())) ;
}


