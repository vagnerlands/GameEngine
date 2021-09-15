#ifndef _ILogger_H_
#define _ILogger_H_

#include "CommonTypes.h"
#include "CGameController.h"

using namespace Types;

namespace Graphics
{
    class Widget;
    class TextArea2DOGL;
}

namespace UtilitiesCore
{
	class ILogger
	{
	public:
        static std::shared_ptr<ILogger> Create(std::shared_ptr<Graphics::Widget> parent);
        static void Destroy();
        static ILogger& Instance();

        void Log(const string& log);
        
        // constructor/destructor
        explicit ILogger(std::shared_ptr<Graphics::Widget> parent);
        virtual ~ILogger();
	private:
        
        static std::shared_ptr<UtilitiesCore::ILogger> s_pInstance;
        std::shared_ptr<Graphics::TextArea2DOGL> m_pLogger;


        // To avoid creation of this object without inheritance
        // copy operations
        ILogger(const ILogger& other) = delete;
        ILogger& operator=(const ILogger& other) = delete;
	};    
}
#define LOG(x) UtilitiesCore::ILogger::Instance().Log(string() + x);
#endif // _ILogger_H_