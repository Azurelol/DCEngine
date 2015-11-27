#include "ReboundBinding.h"

// Components
#include "ReboundComponentsInclude.h"
// Events
#include "ReboundEvents.h"
// Engine
#include "../../Core/Engine/Engine.h" // @todo ew

namespace DCEngine {

  ZilchDefineStaticLibrary(Rebound) {

    ZilchInitializeType(PlayerController);
    ZilchInitializeType(BallController);
    ZilchInitializeType(EnemyController);
    ZilchInitializeType(CameraController);
    ZilchInitializeType(ChargeBar);    
    ZilchInitializeType(HazardArea);
    ZilchInitializeType(MusicManager);
    ZilchInitializeType(PauseManager);
  }

  void ReboundComponentsAddToFactory() {
    // Add the Rebound library
    Daisy->getSystem<Systems::Reflection>()->Handler()->AddLibrary(Rebound::GetLibrary());
    Daisy->getSystem<Systems::Reflection>()->Handler()->Build();
    // Add the components to the engine's component factory map
    auto factory = Daisy->getSystem<Systems::Factory>();
    factory->AddComponentFactory(PlayerController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<PlayerController>>());
    factory->AddComponentFactory(BallController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<BallController>>());
    factory->AddComponentFactory(EnemyController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<EnemyController>>());
    factory->AddComponentFactory(CameraController::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<CameraController>>());
    factory->AddComponentFactory(MusicManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<MusicManager>>());
    factory->AddComponentFactory(PauseManager::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<PauseManager>>());
    factory->AddComponentFactory(ChargeBar::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<ChargeBar>>());
    factory->AddComponentFactory(HazardArea::ZilchGetStaticType(), std::make_unique<Systems::ComponentFactory<HazardArea>>());

  }



}