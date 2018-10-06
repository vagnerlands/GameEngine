#ifndef _IMODEL_H_
#define _IMODEL_H_

#include "CommonTypes.h"
#include "ogltypes.h"

using namespace Types;

namespace Graphics
{

	class IModel
	{
	public:
		IModel();
		virtual ~IModel();
		// create based on SModelData
		virtual bool Create(const SModelData& modelInfo) = 0;
		// render this object as is
		virtual void Draw() = 0;
		// allocate SModelData for custom objects
		virtual SModelData& Allocate() = 0;
		// commit changes
		virtual bool Commit() = 0;
	protected:
		// data to be used, this shall be released once Commit() is called
		SModelData* m_pData;

	private:
		// copy operations
		IModel(const IModel& other);
		IModel& operator=(const IModel& other);
	};


}
#endif // _IMODEL_H_