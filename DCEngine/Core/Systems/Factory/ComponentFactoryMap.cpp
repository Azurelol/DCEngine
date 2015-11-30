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
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Factory.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"

// MACRO: Adds a component factory
//#define DCE_FACTORY_CREATECOMPONENTFACTORY(componentName) \
//AddComponentFactory(::DCEngine::##componentName::ZilchGetStaticType(), std::make_unique<DCEngine::Systems::Factory::ComponentFactory<::DCEngine::##componentName> >())

#define DCE_FACTORY_CREATECOMPONENTFACTORY(componentName) \
AddComponentFactory(componentName::ZilchGetStaticType(), std::make_unique<ComponentFactory<componentName>>()) 

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
      // Space-specific
      AddComponentFactory(SoundSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<SoundSpace>>());
      AddComponentFactory(GraphicsSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<GraphicsSpace>>());
      AddComponentFactory(PhysicsSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<PhysicsSpace>>());
      AddComponentFactory(TimeSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<TimeSpace>>());
      // Physics
      AddComponentFactory(Transform::ZilchGetStaticType(), std::make_unique<ComponentFactory<Transform>>());
      AddComponentFactory(BoxCollider::ZilchGetStaticType(), std::make_unique<ComponentFactory<BoxCollider>>());
      AddComponentFactory(RigidBody::ZilchGetStaticType(), std::make_unique<ComponentFactory<RigidBody>>());      
      // Graphics
      AddComponentFactory(DCEngine::Camera::ZilchGetStaticType(), std::make_unique<ComponentFactory<DCEngine::Camera>>());
      AddComponentFactory(Sprite::ZilchGetStaticType(), std::make_unique<ComponentFactory<Sprite>>());
      AddComponentFactory(SpriteText::ZilchGetStaticType(), std::make_unique<ComponentFactory<SpriteText>>());
      AddComponentFactory(Reactive::ZilchGetStaticType(), std::make_unique<ComponentFactory<Reactive>>());
      // Audio
      AddComponentFactory(SoundEmitter::ZilchGetStaticType(), std::make_unique<ComponentFactory<SoundEmitter>>());
      
      




    }

  }
}