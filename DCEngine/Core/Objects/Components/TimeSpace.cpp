#include "TimeSpace.h"
#include "ObjectReferences.h"

namespace DCEngine {

  void TimeSpace::Initialize() {
    //Connect(space_, Events::LogicUpdate, TimeSpace::OnLogicUpdate);



  }


  void TimeSpace::OnLogicUpdate(Event* updateEvent) {
    Events::LogicUpdate* upcastedEvent = (Events::LogicUpdate*)updateEvent;
    //trace << "TimeSpace::OnLogicUpdate - Dt: " << upcastedEvent->Dt << "\n";   

  }



}