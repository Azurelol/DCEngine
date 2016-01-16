/******************************************************************************/
/*!
@file   Library.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Displays the properties of the currently selected object.
    @todo   Refactor each resource into a template for the type, perhaps?
    */
    /**************************************************************************/
    void Editor::WindowLibrary()
    {
      if (!WidgetLibraryEnabled)
        return;
      
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Library", &WidgetLibraryEnabled);
      
      if (ImGui::TreeNode("Archetype")) {
        for (auto& archetype : *Daisy->getSystem<Content>()->AllArchetypes()) {
          if (ImGui::Selectable(archetype.second->Name().c_str())) {
            SelectedObject = archetype.second.get();
            WindowPropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {          
            CreateFromArchetype(std::string(SelectedObject->Name()));
            break;
          }
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("SpriteLayer")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllSpriteLayers()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            SelectedObject = resource.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("SpriteLayerOrder")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllSpriteLayerOrders()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            SelectedObject = resource.second.get();
            WindowPropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Edit the selected SpriteLayerOrder
            DCTrace << "Editor::WidgetLibrary - Editing: " << resource.second->Name() << "\n";
            SelectedSpriteLayerOrder = resource.second;
            WindowSpriteLayerOrderEditorEnabled = true;
            break;
          }

        }
        ImGui::TreePop();
      }


      if (ImGui::TreeNode("SpriteSource")) {
        for (auto& spriteSrc : *Daisy->getSystem<Content>()->AllSpriteSources()) {
          if (ImGui::Selectable(spriteSrc.second->Name().c_str())) {   
            SelectedObject = spriteSrc.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Font")) {
        for (auto& font : *Daisy->getSystem<Content>()->AllFonts()) {
          if (ImGui::Selectable(font.second->Name().c_str())) {
            SelectedObject = font.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("SoundCue")) {        
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "SoundCues: ");
        for (auto& soundCue : *Daisy->getSystem<Content>()->AllSoundCues()) {
          if (ImGui::Selectable(soundCue.second->Name().c_str())) {
            SelectedObject = soundCue.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Shader")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& shader : *Daisy->getSystem<Content>()->AllShaders()) {
          if (ImGui::Selectable(shader.second->Name().c_str())) {
            SelectedObject = shader.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }
      // 4. Display every level
      if (ImGui::TreeNode("Level")) {
        for (auto& level : *Daisy->getSystem<Content>()->AllLevels()) {
          auto levelName = level.second->Name().c_str();
          if (ImGui::Selectable(levelName) ) {
            SelectedObject = level.second.get();
            WindowPropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Load the selected level
            DCTrace << "Editor::WidgetLibrary - Loading: " << levelName << "\n";
            LoadLevel(levelName);
            break;
          }
        }
        
        //ScannedForLevels = false;
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("CollisionGroup")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllCollisionGroups()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            SelectedObject = resource.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      if (ImGui::TreeNode("CollisionTable")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllCollisionTables()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            SelectedObject = resource.second.get();
            WindowPropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Edit the selected collision table
            DCTrace << "Editor::WidgetLibrary - Editing: " << resource.second->Name() << "\n";
            SelectedCollisionTable = resource.second.get();
            WindowCollisionTableEditorEnabled = true;
            break;
          }

        }
        ImGui::TreePop();
      }


      if (ImGui::TreeNode("PhysicsMaterial")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllPhysicsMaterials()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            SelectedObject = resource.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }


      if (ImGui::TreeNode("ZilchScript")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& script : *Daisy->getSystem<Content>()->AllZilchScripts()) {
          if (ImGui::Selectable(script.second->Name().c_str())) {
            SelectedObject = script.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      ImGui::End();
    }



  }
}
