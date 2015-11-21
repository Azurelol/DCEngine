#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void Editor::WindowConsole()
    {
      if (!WindowConsoleEnabled)
        return;

      ImGui::Begin("Console", &WindowConsoleEnabled);
      // The console needs to read the DCTrace lines while active.
      // Or it could read the log file?
      
      ImGui::Text("Console text here");

      ImGui::End();
      


    }

  }
}