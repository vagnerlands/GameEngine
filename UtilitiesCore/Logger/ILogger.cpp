#include "ILogger.h"
#include "RenderUI.h"
#include "OpenGL/TextArea2DOGL.h"

// STATIC SESSION //
std::shared_ptr<UtilitiesCore::ILogger> UtilitiesCore::ILogger::s_pInstance = nullptr;
std::shared_ptr<UtilitiesCore::ILogger> UtilitiesCore::ILogger::Create(std::shared_ptr<Graphics::Widget> parent)
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = std::make_shared<UtilitiesCore::ILogger>(parent);
    }
    return s_pInstance;
}

void UtilitiesCore::ILogger::Destroy()
{
    s_pInstance = nullptr;
}

// CONSTRUCTOR SESSION //
UtilitiesCore::ILogger::ILogger(std::shared_ptr<Graphics::Widget> parent) :
    m_pLogger(std::make_shared<Graphics::TextArea2DOGL>(IvVector4(0.f, 0.f, 0.f, 0.f), IvVector3(1.f, 1.f, 1.f), 0.25f))
{
    m_pLogger->SetLocation({ 0.f, 0.f });
    parent->Append(m_pLogger, Graphics::Widget::eFillStyle_Fill);
    Graphics::RenderUI::Instance().Add("logger_text_area", m_pLogger);
}
UtilitiesCore::ILogger::~ILogger()
{}

// APIs //
void UtilitiesCore::ILogger::Log(const string & log)
{
    m_pLogger->AddLine(log);
}

UtilitiesCore::ILogger&
UtilitiesCore::ILogger::Instance()
{    
    return *s_pInstance;
}
