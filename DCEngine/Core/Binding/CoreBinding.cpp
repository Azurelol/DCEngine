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
    ZilchInitializeType(GameSession);
    ZilchInitializeType(Space);
    ZilchInitializeType(GameObject);

    /*===================*
    *     Resources      *
    *===================*/
    ZilchInitializeType(SpriteSource);
    ZilchInitializeType(SoundCue);

    /*===================*
    *     Components     *
    *===================*/
    // Space
    ZilchInitializeType(PhysicsSpace);
    ZilchInitializeType(GraphicsSpace);
    ZilchInitializeType(SoundSpace);
    ZilchInitializeType(TimeSpace);
    // Physics
    ZilchInitializeType(Transform);
    ZilchInitializeType(RigidBody);
    ZilchInitializeType(BoxCollider);
    // Graphics
    ZilchInitializeType(Sprite);
    ZilchInitializeType(SpriteText);
    ZilchInitializeType(Camera);
    ZilchInitializeType(EditorCameraController);
    ZilchInitializeType(Reactive);
    // Audio
    ZilchInitializeType(SoundSpace);
    ZilchInitializeType(SoundEmitter);
    

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