/******************************************************************************/
/*!
@file   Objects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's objects widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Displays a list of objects in the default space.
    */
    /**************************************************************************/
    void Editor::WidgetObjects()
    {
      if (!WidgetObjectsEnabled)
        return;

      // Set the position of the window
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      // Title
      ImGui::Begin("Objects", &WidgetObjectsEnabled);
      // Print every name
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      for (auto& object : *objects) {
        auto ObjectName = object->Name().c_str();
        // If the user has selected the GameObject.
        if (ImGui::Button(ObjectName)) {
          SelectedObject = object;
          WindowPropertiesEnabled = true;
        }
          
        //ImGui::Text(object->Name().c_str());
      }

      // Ends the window
      ImGui::End();

    }


  }
}