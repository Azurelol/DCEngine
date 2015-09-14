#include "TimeSpace.h"



namespace DCEngine {

  void TimeSpace::Initialize() {
    auto space = Owner();

  }

  void TimeSpace::OnLogicUpdate(Event * updateEvent) {
    if (TRACE_UPDATE)
      trace << "TimeSpace::OnLogicUpdate\n";

  }

}