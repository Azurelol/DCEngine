/******************************************************************************/
/*!
@file   WindowConsole.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/



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