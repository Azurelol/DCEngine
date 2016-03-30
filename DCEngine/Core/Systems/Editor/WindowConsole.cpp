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
#include <sstream>
#include <climits>


namespace DCEngine {
  

  namespace Systems {

    void Editor::WindowConsole()
    {
      if (!Windows.ConsoleEnabled)
        return;

      ImGui::SetNextWindowPosCenter(ImGuiSetCond_Once);
      ImGui::Begin("Console", &Windows.ConsoleEnabled);

      Settings.ConsoleWindowStringLength = strlen(DCEngine::Debug::traceObj->stream.str().c_str());

      ImGuiTextBuffer log;
      log.append(DCEngine::Debug::traceObj->stream.str().c_str());
      ImGui::TextUnformatted(log.begin(), log.end());
      
      if (Settings.ConsoleWindowStringLength > Settings.PreviousConsoleWindowStringLength)
      {
        ImGui::SetScrollY(INT_MAX);
        Settings.PreviousConsoleWindowStringLength = strlen(DCEngine::Debug::traceObj->stream.str().c_str());
      }
     
      ImGui::End();
      


    }
  }
}