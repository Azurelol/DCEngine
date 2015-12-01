/*****************************************************************************/
/*!
@file   WindowsCreate.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Displays a window allowing the user to create an object
            from a list of archetypes.
    */
    /**************************************************************************/
    void Editor::WindowCreateFromArchetype()
    {
      if (!WindowCreateFromArchetypeEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Create from Archetype", &WindowCreateFromArchetypeEnabled);
      {
        // Display a list of all available archetypes
          // Query the Content system for the current list of archetypes
          static bool ScannedForArchetypes;
          if (!ScannedForArchetypes) {
            Daisy->getSystem<Content>()->ScanForArchetypes();
            ScannedForArchetypes = true;
          }

          // Display all archetypes.
          static char archetypeName[32];
          for (auto& archetype : *Daisy->getSystem<Content>()->AllArchetypes()) {
            if (ImGui::Selectable(archetype.second->Name().c_str())) {              
              strcpy(archetypeName, archetype.second->Name().c_str());
            }
          }

          ImGui::InputText("##archetype", archetypeName, IM_ARRAYSIZE(archetypeName));

          if (ImGui::Button("Create")) {
            CreateFromArchetype(std::string(archetypeName));
            WindowCreateFromArchetypeEnabled = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            WindowCreateFromArchetypeEnabled = false;
          }

      }      
      ImGui::End();
      //CurrentSpace->add
    }

  }
}