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
    ZilchInitializeType(ChargeBar);
    ZilchInitializeType(CameraController);
    ZilchInitializeType(HazardArea);

  }

  void ReboundComponentsAddToFactory() {

    // Add the Rebound library
    Daisy->getSystem<Systems::Reflection>()->Handler()->AddLibrary(Rebound::GetLibrary());
    Daisy->getSystem<Systems::Reflection>()->Handler()->Build();

    // Add the components to the engine's component factory map
    auto factory = Daisy->getSystem<Systems::Factory>();
    factory->AddComponentFactory(PlayerController::ZilchGetStaticType(), 
                                 std::make_unique<Systems::ComponentFactory<PlayerController>>());

  }



}