#ifndef _WidgetTransforms_H_
#define _WidgetTransforms_H_

#include "IvVector2.h"
#include "IvMatrix44.h"

using namespace Types;
namespace Graphics
{
    // interface to render 2d objects on the screen hud
	class WidgetTransforms
	{
    protected:
        IvMatrix44                  m_transforms; // transformations
        const WidgetTransforms*     m_pParent = nullptr;
        IvVector2                   m_location = { 0.F, 0.F };
        IvVector2                   m_size{ 0.F, 0.F };

        IvVector2 findParentsAccumulatedLocation(const WidgetTransforms* pParent)
        {
            if (pParent == nullptr)
            {
                return IvVector2(0.F, 0.F);
            }
            return IvVector2(pParent->GetLocation().GetX(), pParent->GetLocation().GetY()) + findParentsAccumulatedLocation(pParent->m_pParent);
        }

        void updateModel()
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
            //rotateModel.Rotation(m_rota);
            // scale transformation
            scaleModel.Scaling(IvVector3(m_size.GetX(), m_size.GetY(), 1.f));

            // transverse to all parent nodes, accumulating their locations to find the relative location to render this window...
            IvVector2 parentsLocation = findParentsAccumulatedLocation(m_pParent);
            translateModel.Translation(IvVector3(m_location.GetX() + parentsLocation.GetX(), m_location.GetY() + parentsLocation.GetY(), 0.f));

            // combine all transformations - in case this is a child, it takes the parent transformations to calculate 
            // it's own             
            m_transforms = translateModel * scaleModel * rotateModel;
        }
	public:
        virtual ~WidgetTransforms() {}
        WidgetTransforms& SetLocation(const IvVector2& location)
        {
            m_location = location;
            return *this;
        }
        WidgetTransforms& SetSize(const IvVector2& size)
        {
            m_size = size;
            return *this;
        }
        const IvVector2& GetLocation() const
        {
            return m_location;
        }
        const IvVector2& GetSize() const
        {
            return m_size;
        }
        void SetParent(const WidgetTransforms* pParent)
        {
            m_pParent = pParent;
        }
	};
}
#endif // _WidgetTransforms_H_