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
// Used to add component properties
#include "..\Systems\Reflection\ZilchInterfaceUtilities.h"
// Core Events
#include "..\EventsInclude.h"
#include "CoreBindingEventStrings.h"
// Engine
#include "..\Engine\Engine.h"
//Daisy Vector
#include "../DaisyVector.h"

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
    ZilchInitializeType(Engine);
    ZilchInitializeType(Entity);
    ZilchInitializeType(Resource);
    ZilchInitializeType(Mouse);
    ZilchInitializeType(Keyboard);
    ZilchInitializeType(Component);
    ZilchInitializeType(ZilchComponent);
    ZilchInitializeType(GameSession);
    ZilchInitializeType(Space);
    ZilchInitializeType(GameObject);
    /*===================*
    *     Collision      *
    *===================*/
    ZilchInitializeType(Ray);
    ZilchInitializeType(CastResult);
    ZilchInitializeType(CastFilter);
    ZilchInitializeType(CollisionBlock);
    ZilchInitializeType(CollisionFilter);
    ZilchInitializeType(CollisionTable);
    //ZilchInitializeType(DaisyVector<CastResult>);
    /*===================*
    *     Resources      *
    *===================*/
    ZilchInitializeType(SpriteSource);
    ZilchInitializeType(Bank);
    ZilchInitializeType(SoundCue);
    ZilchInitializeType(SoundInstance);
    ZilchInitializeType(CollisionGroup);
    ZilchInitializeType(CollisionTable);
    ZilchInitializeType(ZilchScript);
    ZilchInitializeType(PhysicsMaterial);
    ZilchInitializeType(Font);
    ZilchInitializeType(SpriteLayer);
    ZilchInitializeType(SpriteLayerOrder);
    /*===================*
    *      Actions      *
    *===================*/
    ZilchInitializeType(Action);
    ZilchInitializeType(ActionSet);
    ZilchInitializeType(ActionGroup);
    ZilchInitializeType(ActionSequence);
    ZilchInitializeType(ActionsOwner);
    ZilchInitializeType(Actions);
    /*===================*
    *      Events        *
    *===================*/
    ZilchInitializeType(Event);
    ZilchInitializeType(EventStrings);
    //Game
    ZilchInitializeType(Events::GameFocusIn);
    ZilchInitializeType(Events::GameFocusOut);
    ZilchInitializeType(Events::GameLoad);
    ZilchInitializeType(Events::GameRequestQuit);
    ZilchInitializeType(Events::GameStarted);
    ZilchInitializeType(Events::GameEnded);
    ZilchInitializeType(Events::GameSetup);
    //Key
    ZilchInitializeType(Events::KeyDown);
    ZilchInitializeType(Events::KeyUp);
    //Mouse
    ZilchInitializeType(Events::MouseDown);
    ZilchInitializeType(Events::MouseUp);
    ZilchInitializeType(Events::MouseEnter);
    ZilchInitializeType(Events::MouseExit);
    ZilchInitializeType(Events::MouseUpdate);
    ZilchInitializeType(Events::MouseClickedOn);
    ZilchInitializeType(Events::MouseScroll);
    //Collision
    ZilchInitializeType(Events::CollisionStarted);
    ZilchInitializeType(Events::CollisionEnded);
    ZilchInitializeType(Events::CollisionPersisted);
    //Update
    ZilchInitializeType(Events::LogicUpdate);
    ZilchInitializeType(Events::FrameUpdate);
    ZilchInitializeType(Events::PhysicsUpdate);
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
    ZilchInitializeType(Components::Orientation);
    // Graphics
    ZilchInitializeType(Components::Sprite);
    ZilchInitializeType(Components::SpriteText);
    ZilchInitializeType(Components::Camera);
    ZilchInitializeType(Components::SpriteParticleSystem);
    ZilchInitializeType(Components::ParticleEmitter);
    ZilchInitializeType(Components::ParticleColorAnimator);
    ZilchInitializeType(Components::LinearParticleAnimator);    
    ZilchInitializeType(Components::Light);
    ZilchInitializeType(Components::Reactive);
    // Audio
    ZilchInitializeType(Components::SoundEmitter);
    // Tools
    ZilchInitializeType(Components::EditorCameraController);
    ZilchInitializeType(Components::TransformTool);   
    // Debug
    ZilchInitializeType(Components::DebugActions);
    ZilchInitializeType(Components::DebugAudio);
    // Zilch
    ZilchInitializeType(ZilchComponent);
    ZilchInitializeType(ZilchEvent);    

    // Setup extension properties for Entity
    auto interface = Systems::ZilchInterface::Get();
    auto& boundTypes = builder.BoundTypes.values();
    while (!boundTypes.empty()) {
      interface.SetupTypeProperty(boundTypes.front(), ZilchTypeId(Component), ZilchTypeId(Entity), boundTypes.front(),
                                  &builder, Systems::GetNativeComponent);
      boundTypes.popFront();
    }
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