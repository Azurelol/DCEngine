#include "TimeSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  void TimeSpace::Initialize() {
    //Connect(SpaceRef, Events::LogicUpdate, TimeSpace::OnLogicUpdate);



  }

  void TimeSpace::Serialize(Json::Value & root) {
  }

  void TimeSpace::Deserialize(Json::Value & root) {
  }


  void TimeSpace::OnLogicUpdate(Events::LogicUpdate* updateEvent) {
    //Events::LogicUpdate* upcastedEvent = (Events::LogicUpdate*)updateEvent;
    //trace << "TimeSpace::OnLogicUpdate - Dt: " << upcastedEvent->Dt << "\n";   

  }



}