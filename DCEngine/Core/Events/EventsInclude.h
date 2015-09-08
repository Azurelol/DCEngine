#pragma once

/********
  UPDATE
********/
#include "UpdateEvent.h"

/*******
  INPUT
*******/
#include "KeyboardEvent.h"


namespace DCEngine {

  enum class EventType {
    UpdateEvent,
    KeyboardEvent,

  };

  // Expansion macro (Events.SampleEvent)
  //#define Events \ EventType;
  //#define Events.(eventType) \ (Events::##eventType)
  //#define Events.(eventType) \ (EventType::eventType)
  //using Events. = EventType::;


}

