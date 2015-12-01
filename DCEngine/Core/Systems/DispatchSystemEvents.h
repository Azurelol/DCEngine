#pragma once

namespace DCEngine {
  namespace Systems {

    class DispatchSystemEvents {
    public:

      static void EngineResume();
      static void EnginePause();
      static void EngineExit();
      
      static void SetWindowCaption(std::string name);
      static void WindowLostFocus();
      static void WindowGainedFocus();

    };

  }

 

}