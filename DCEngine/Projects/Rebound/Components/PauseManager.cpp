#include "PauseManager.h"

// Engine
#include "../../../Core/Engine/Engine.h"

namespace DCEngine {


  /**************************************************************************/
  /*!
  @brief  Displays a Pause Menu.
  @todo   Implement the pause menu in a different way.
  */
  /**************************************************************************/
  void PauseMenu()
  {
    // Pause the engine (Physics, Input, Events)
    auto pause = new Events::EnginePause();
    Daisy->Dispatch<Events::EnginePause>(pause);
    delete pause;

    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);

    
    ImGui::Begin("Pause Menu", &PauseMenuEnabled);
    if (ImGui::Button("Pause")) {

    }
    if (ImGui::Button("Resume Game")) {
      // Unpause the engine (Physics, Input, Events)
      auto resume = new Events::EngineResume();
      Daisy->Dispatch<Events::EngineResume>(resume);
      delete resume;
      PauseMenuEnabled = false;
    }

    if (ImGui::Button("How to Play")) {

    }


    if (ImGui::Button("Quit Game")) {
      // Destructive confirmation
      auto exitEvent = new Events::EngineExit();
      Daisy->Dispatch<Events::EngineExit>(exitEvent);
      delete exitEvent;
    }

    ImGui::End();

  }


}