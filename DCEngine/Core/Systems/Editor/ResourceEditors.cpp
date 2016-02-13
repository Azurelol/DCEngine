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
      if (!Windows.CollisionTableEditorEnabled)
        return;

      std::string title = "CollisionTable Editor - " + SelectedCollisionTable->Name();
      ImGui::Begin(title.c_str(), &Windows.CollisionTableEditorEnabled);
      

      auto &Groups = Daisy->getSystem<Content>()->getCollisionTable(SelectedCollisionTable->Name())->GetGroups();
      
      auto &Pairs = Daisy->getSystem<Content>()->getCollisionTable(SelectedCollisionTable->Name())->GetPairs();
      // Create an array from all available collision groups (so that we have random access)

      static char GroupName[64];

      for (auto& group : Groups)
      {
        Math::ClampString(group.Name().c_str(), GroupName, 4);
        ImGui::Text(GroupName);
        ImGui::SameLine();
      }

      ImGui::Text(" ");

     static CollisionFilter *Selection = &(Pairs[1]);

      for (auto& group : Groups) 
      {
        Math::ClampString(group.Name().c_str(), GroupName, 4);
        ImGui::Text(GroupName);
        for (auto &pair : Pairs)
        {
          if (pair.Pairing.first == group.Name() || pair.Pairing.second == group.Name())
          {
            ImGui::SameLine();
            if (ImGui::Button(" "))
            {
              Selection = &pair;
            }
          }
        }
      }

      ImGui::Separator();

      ImGui::Text("Pairing: ");

      ImGui::SameLine();

      ImGui::Text((Selection->Pairing.first + std::string(" and ") + Selection->Pairing.second).c_str());

      ImGui::Text("Resolution:");

      ImGui::SameLine();
      
      switch (Selection->CollisionFlag)
      {
        case CollisionFlag::Resolve:
          ImGui::Text("Resolve");
          break;
        case CollisionFlag::SkipResolution:
          ImGui::Text("Skip Resolution");
          break;
        case CollisionFlag::SkipDetecting:
          ImGui::Text("Skip Detection");
          break;
      }

      if (ImGui::Button("Resolve"))
      {
        Selection->CollisionFlag = CollisionFlag::Resolve;
      }
      ImGui::SameLine();
      if (ImGui::Button("Skip Resolution"))
      {
        Selection->CollisionFlag = CollisionFlag::SkipResolution;
      }
      ImGui::SameLine();
      if (ImGui::Button("Skip Detection"))
      {
        Selection->CollisionFlag = CollisionFlag::SkipDetecting;
      }

      
      ImGui::Text("Collision Started Event:");

      ImGui::SameLine();

      switch (Selection->CollisionStartBlock.SendEventsToA)
      {
      case true:
        ImGui::Text("True");
        break;
      case false:
        ImGui::Text("False");
        break;
      }

      if (ImGui::Button("Resolve"))
      {
        Selection->CollisionFlag = CollisionFlag::Resolve;
      }
      ImGui::SameLine();
      if (ImGui::Button("Skip Resolution"))
      {
        Selection->CollisionFlag = CollisionFlag::SkipResolution;
      }
      ImGui::SameLine();
      if (ImGui::Button("Skip Detection"))
      {
        Selection->CollisionFlag = CollisionFlag::SkipDetecting;
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
      if (!Windows.SpriteLayerOrderEditorEnabled)
        return;

      std::string title = "SpriteLayerOrder Editor - " + SelectedSpriteLayerOrder->Name();
      ImGui::SetNextWindowSize(ImVec2(400, 200));
      ImGui::Begin(title.c_str(), &Windows.SpriteLayerOrderEditorEnabled, ImGuiSetCond_FirstUseEver);


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
