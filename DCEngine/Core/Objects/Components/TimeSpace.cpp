#include "TimeSpace.h"

#include "../../Engine/Engine.h" // TEMPORARY

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy; // TEMPORARY

  void TimeSpace::Initialize() {
    Connect(space_, Events::LogicUpdate, TimeSpace::OnLogicUpdate);
  }


  void TimeSpace::OnLogicUpdate(Event* updateEvent) {
    Events::LogicUpdate* upcastedEvent = (Events::LogicUpdate*)updateEvent;
    trace << "TimeSpace::OnLogicUpdate - Dt: " << upcastedEvent->Dt << "\n";
    

  }

}