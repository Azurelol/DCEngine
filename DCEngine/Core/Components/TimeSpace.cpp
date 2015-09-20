#include "TimeSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  void TimeSpace::Initialize() {
    //Connect(space_, Events::LogicUpdate, TimeSpace::OnLogicUpdate);



  }


  void TimeSpace::OnLogicUpdate(Events::LogicUpdate* updateEvent) {
    //Events::LogicUpdate* upcastedEvent = (Events::LogicUpdate*)updateEvent;
    //trace << "TimeSpace::OnLogicUpdate - Dt: " << upcastedEvent->Dt << "\n";   

  }



}