/******************************************************************************/
/*!
@file   ComponentFactory.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/21/2015
@brief  The Component Factory interacts with BoundTypes to construct a component
        from a BoundType* and returns it via unique_ptr.
*/
/******************************************************************************/
#pragma once
#include "../../Engine/Types.h"
#include "../../ComponentsInclude.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    @class  The AbstractComponentFactory class is the one we have containers of.
    */
    /**************************************************************************/
    class AbstractComponentFactory {
    public:
      virtual std::unique_ptr<Component> ConstructComponent(Entity& owner) = 0;
    };

    /**************************************************************************/
    /*!
    @class  Every derived 'Component' has its own instantiated factory that is
            used to make instances of that component type.
    */
    /**************************************************************************/
    template <typename ComponentClass>
    class ComponentFactory : public AbstractComponentFactory {
    public:
      std::unique_ptr<Component> ConstructComponent(Entity& owner) {
        return std::make_unique<ComponentClass>(owner);        
      }
    };

    using ComponentMap = std::unordered_map<Zilch::BoundType*, std::unique_ptr<AbstractComponentFactory>>;

  }
}