/******************************************************************************/
/*!
@file   CollisionTable.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/18/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    void Editor::WindowCollisionTableEditor()
    {
      if (!WindowCollisionTableEditorEnabled)
        return;

      std::string title = "Collision Table Editor - " + SelectedCollisionTable->Name();
      ImGui::Begin(title.c_str(), &WindowCollisionTableEditorEnabled);
      
      // Create an array from all available collision groups (so that we have random access)
      std::vector<CollisionGroupPtr> groups;
      for (auto& group : *Daisy->getSystem<Content>()->AllCollisionGroups()) {
        groups.push_back(group.second);
      }
      // For every collision group, create a row
      for (unsigned int i = 0; i < groups.size(); ++i) {        
        ImGui::Text(groups[i]->Name().c_str());
        for (unsigned int j = i + 1; j < groups.size(); ++j) {
          ImGui::SameLine();
          if (ImGui::Button(groups[j]->Name().c_str())) {
            // Display a dialog of the pair between this group and the other?
          }
        }
      }

      ImGui::End();

    }


  }
}
