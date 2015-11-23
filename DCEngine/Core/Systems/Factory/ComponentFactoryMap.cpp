/******************************************************************************/
/*!
@file   ComponentFactoryMap.cpp
@author Christian Sagel
@par    email: blaine.reiner\@digipen.edu
@par    DigiPen login: blaine.reiner
@date   11/20/2015
@brief  Constructs a component factory for every bound component class in
        the engine. This allows them to be created at runtime through
        reflection.
*/
/******************************************************************************/
#include "Factory.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"

// MACRO: Adds a component factory
#define DCE_FACTORY_CREATECOMPONENTFACTORY(componentName) \
AddComponentFactory(::DCEngine::##componentName::ZilchGetStaticType(), std::make_unique<DCEngine::Systems::Factory::ComponentFactory<::DCEngine::##componentName> >())

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Constructs a component factory for every bound component type.
    */
    /**************************************************************************/
    void Factory::ConstructComponentFactoryMap()
    {
      // Loop through every known bound type...
      auto components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
      for (auto component : components) {
        // And adds it to the component factory map
        auto name = std::string(component->Name.c_str());        
        //DCE_FACTORY_CREATECOMPONENTFACTORY(name);
      }
      
      DCTrace << "Factory::ConstructComponentFactoryMap - Constructing all component factories \n";
      AddComponentFactory(Transform::ZilchGetStaticType(), std::make_unique<ComponentFactory<Transform>>());
      AddComponentFactory(Sprite::ZilchGetStaticType(), std::make_unique<ComponentFactory<Sprite>>());
      AddComponentFactory(BoxCollider::ZilchGetStaticType(), std::make_unique<ComponentFactory<BoxCollider>>());
      AddComponentFactory(RigidBody::ZilchGetStaticType(), std::make_unique<ComponentFactory<RigidBody>>());
      AddComponentFactory(DCEngine::Camera::ZilchGetStaticType(), std::make_unique<ComponentFactory<DCEngine::Camera>>());



    }

  }
}