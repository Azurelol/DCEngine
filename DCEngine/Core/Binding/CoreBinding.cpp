/******************************************************************************/
/*!
@file   CoreBinding.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/07/2015
@brief  This file includes the definition of the static library for all the
        Core engine classes (Object, Entities, Components).
        It will also initialize them.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Precompiled.h"
#include "CoreBinding.h"

// Core Objects
#include "..\Objects/Object.h"
// Core Entities
#include "..\Objects\Entities\EntitiesInclude.h"
// Core Components
#include "..\ComponentsInclude.h"
// Engine
#include "..\Engine\Engine.h"

namespace DCEngine {  

  ZilchDefineStaticLibrary(DCEngineCore) {

    /*
    We have to initialize all the types that we have bound to our library.
    Ideally we could use pre-main or automatic registration, but there's a major issues where
    compilers automatically remove "unreferenced" classes, even if they are referenced
    by globals/pre-main initializations. This method ensures that all classes will be properly bound.
    */
    
    /*===================*
    *     Objects       *
    *===================*/
    ZilchInitializeType(Object);
    ZilchInitializeType(Entity);
    ZilchInitializeType(Resource);
    ZilchInitializeType(Component);
    ZilchInitializeType(ZilchComponent);
    ZilchInitializeType(GameSession);
    ZilchInitializeType(Space);
    ZilchInitializeType(GameObject);

    /*===================*
    *     Resources      *
    *===================*/
    ZilchInitializeType(SpriteSource);
    ZilchInitializeType(SoundCue);
    ZilchInitializeType(CollisionGroup);
    ZilchInitializeType(CollisionTable);
    ZilchInitializeType(ZilchScript);
    ZilchInitializeType(PhysicsMaterial);
    ZilchInitializeType(Font);
    ZilchInitializeType(SpriteLayer);
    ZilchInitializeType(SpriteLayerOrder);

    /*===================*
    *     Components     *
    *===================*/
    // Space
    ZilchInitializeType(Components::PhysicsSpace);
    ZilchInitializeType(Components::GraphicsSpace);
    ZilchInitializeType(Components::CameraViewport);
    ZilchInitializeType(Components::SoundSpace);
    ZilchInitializeType(Components::TimeSpace);
    // Physics
    ZilchInitializeType(Components::Transform);
    ZilchInitializeType(Components::RigidBody);
    ZilchInitializeType(Components::BoxCollider);
    // Graphics
    ZilchInitializeType(Components::Sprite);
    ZilchInitializeType(Components::SpriteText);
    ZilchInitializeType(Components::Camera);
    
    ZilchInitializeType(Components::Reactive);
    // Audio
    ZilchInitializeType(Components::SoundEmitter);
    // Tools
    ZilchInitializeType(Components::EditorCameraController);
    ZilchInitializeType(Components::TransformTool);
    

  }


  /**************************************************************************/
  /*!
  @brief  Constructs a component factory for every bound component type.
  */
  /**************************************************************************/
  void Systems::Factory::ConstructComponentFactoryMap()
  {
    // Loop through every known bound type...
    //auto components = Daisy->getSystem<Systems::Reflection>()->AllComponents();
    //for (auto component : components) {
    //  // And adds it to the component factory map
    //  auto name = std::string(component->Name.c_str());
    //  //DCE_FACTORY_CREATECOMPONENTFACTORY(name);
    //}

    DCTrace << "Factory::ConstructComponentFactoryMap - Constructing all component factories \n";
    // Space
    AddComponentFactory(Components::SoundSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::SoundSpace>>());
    AddComponentFactory(Components::GraphicsSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::GraphicsSpace>>());
    AddComponentFactory(Components::PhysicsSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::PhysicsSpace>>());
    AddComponentFactory(Components::TimeSpace::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::TimeSpace>>());
    // Physics
    AddComponentFactory(Components::Transform::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::Transform>>());
    AddComponentFactory(Components::BoxCollider::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::BoxCollider>>());
    AddComponentFactory(Components::RigidBody::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::RigidBody>>());
    // Graphics
    AddComponentFactory(Components::Camera::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::Camera>>());
    AddComponentFactory(Components::Sprite::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::Sprite>>());
    AddComponentFactory(Components::SpriteText::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::SpriteText>>());
    AddComponentFactory(Components::Reactive::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::Reactive>>());
    // Audio
    AddComponentFactory(Components::SoundEmitter::ZilchGetStaticType(), std::make_unique<ComponentFactory<Components::SoundEmitter>>());






  }

}