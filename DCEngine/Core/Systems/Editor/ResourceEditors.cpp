/******************************************************************************/
/*!
@file   ResourceEditors.cpp
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

    /**************************************************************************/
    /*!
    @brief  Displays the CollisionTable editor.
    */
    /**************************************************************************/
    void Editor::WindowCollisionTableEditor()
    {
      if (!WindowCollisionTableEditorEnabled)
        return;

      std::string title = "CollisionTable Editor - " + SelectedCollisionTable->Name();
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

    /**************************************************************************/
    /*!
    @brief  Displays the SpriteLayerOrder editor.
    */
    /**************************************************************************/
    void Editor::WindowSpriteLayerOrderEditor()
    {
      if (!WindowSpriteLayerOrderEditorEnabled)
        return;

      std::string title = "SpriteLayerOrder Editor - " + SelectedSpriteLayerOrder->Name();
      ImGui::SetNextWindowSize(ImVec2(400, 200));
      ImGui::Begin(title.c_str(), &WindowSpriteLayerOrderEditorEnabled, ImGuiSetCond_FirstUseEver);


      // Display all present SpriteLayers
      static SpriteLayerHandle selectedSpriteLayer;
      static bool selected;
      for (auto& layer : SelectedSpriteLayerOrder->getList()) {
        if (ImGui::Selectable(layer.c_str(), selectedSpriteLayer == layer)) {
          selectedSpriteLayer = layer;
        }
      }

      // Add
      if (ImGui::TreeNode("Add SpriteLayer...")) {
        // Grab a list of all SpriteLayers
        std::vector<SpriteLayerHandle> layers;
        for (auto& layer : *Daisy->getSystem<Content>()->AllSpriteLayers()) {
          // If the layer is already present, skip it.
          auto& layerName = layer.second->Name();
          if (SelectedSpriteLayerOrder->Has(layerName))
            continue;
          // If not present, allow adding it!
          if (ImGui::Selectable(layer.second->Name().c_str())) {
            SelectedSpriteLayerOrder->Add(layerName);
          }
        }
        ImGui::TreePop();
      }

      if (ImGui::Button("Back")) {
        SelectedSpriteLayerOrder->Move(selectedSpriteLayer, Direction::Up);        
      }
      ImGui::SameLine();
      if (ImGui::Button("Front")) {
        SelectedSpriteLayerOrder->Move(selectedSpriteLayer, Direction::Down);
      }
      ImGui::SameLine();
      if (ImGui::Button("Remove")) {
        SelectedSpriteLayerOrder->Remove(selectedSpriteLayer);
      }

      ImGui::End();
    }


  }
}
