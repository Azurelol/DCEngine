/*****************************************************************************/
/*!
@file   ReboundBinding.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ReboundBinding.h"

// Components
#include "ReboundComponentsInclude.h"
// Events
#include "ReboundEvents.h"
// Engine
#include "../../Core/Engine/Engine.h" // @todo ew

namespace DCEngine {

  ZilchDefineStaticLibrary(Rebound) {

    ZilchInitializeType(Components::PlayerController);
    ZilchInitializeType(Components::BallController);
    ZilchInitializeType(Components::EnemyController);
    ZilchInitializeType(Components::CameraController);
    ZilchInitializeType(Components::ChargeBar);    
	  ZilchInitializeType(Components::Fade);
    ZilchInitializeType(Components::HazardArea);
	  ZilchInitializeType(Components::MoveToLocation);
    ZilchInitializeType(Components::MusicManager);
    ZilchInitializeType(Components::PauseManager);
	  ZilchInitializeType(Components::LevelManager);
    ZilchInitializeType(Components::MainMenuManager);
	  ZilchInitializeType(Components::ErraticDoor);
	  ZilchInitializeType(Components::TutorialTextLogic);
	  ZilchInitializeType(Components::LockField);
	  ZilchInitializeType(Components::PowerField);
  }

  void ReboundComponentsAddToFactory() {
    // Add the Rebound library
    Daisy->getSystem<Systems::Reflection>()->Handler()->AddLibrary(Rebound::GetLibrary());
    Daisy->getSystem<Systems::Reflection>()->Handler()->Build();
    // Add the components to the engine's component factory map
    auto factory = Daisy->getSystem<Systems::Factory>();
    factory->AddComponentFactory(Components::PlayerController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::PlayerController>>());
    factory->AddComponentFactory(Components::BallController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::BallController>>());
    factory->AddComponentFactory(Components::EnemyController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::EnemyController>>());
    factory->AddComponentFactory(Components::CameraController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::CameraController>>());
    factory->AddComponentFactory(Components::MusicManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MusicManager>>());
    factory->AddComponentFactory(Components::PauseManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::PauseManager>>());
    factory->AddComponentFactory(Components::ChargeBar::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::ChargeBar>>());
    factory->AddComponentFactory(Components::HazardArea::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::HazardArea>>());
	  factory->AddComponentFactory(Components::Fade::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::Fade>>());
	  factory->AddComponentFactory(Components::LevelManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::LevelManager>>());
    factory->AddComponentFactory(Components::MainMenuManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MainMenuManager>>());
	  factory->AddComponentFactory(Components::MoveToLocation::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::MoveToLocation>>());
	  factory->AddComponentFactory(Components::ErraticDoor::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::ErraticDoor>>());
	  factory->AddComponentFactory(Components::TutorialTextLogic::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::TutorialTextLogic>>());
	  factory->AddComponentFactory(Components::LockField::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::LockField>>());
	  factory->AddComponentFactory(Components::PowerField::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<Components::PowerField>>());
  }



}