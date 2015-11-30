#include "DispatchSystemEvents.h"
#include "../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void DispatchSystemEvents::EngineResume()
    {

    }

    void DispatchSystemEvents::EnginePause()
    {

    }


    void DispatchSystemEvents::SetWindowCaption(std::string caption)
    {
      auto event = new Events::SetWindowCaption;      
      event->Caption = caption;
      Daisy->Dispatch<Events::SetWindowCaption>(event);
      delete event;
    }


  }
}


