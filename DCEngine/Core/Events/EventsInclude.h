#pragma once

/********
  UPDATE
********/
#include "LogicUpdate.h"

/*******
  INPUT
*******/
#include "KeyboardEvent.h"


namespace DCEngine {

  enum class EventType {
    LogicUpdate,
    KeyboardEvent,
    PleaseWorkEvent,

  };

  // Expansion macro (Events.SampleEvent)
  //#define Events \ EventType;
  //#define Events.(eventType) \ (Events::##eventType)
  //#define Events.(eventType) \ (EventType::eventType)
  //using Events. = EventType::;


}

