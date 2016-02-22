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
      if (!Windows.LibraryEnabled)
        return;
      
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Library", &Windows.LibraryEnabled);           
      
      if (ImGui::TreeNode("Archetype")) {
        for (auto& archetype : *Daisy->getSystem<Content>()->AllArchetypes()) {
          if (ImGui::Selectable(archetype.second->Name().c_str())) {
            Archetypes.Select(archetype.second->Name());
            //Select(archetype.second.get());
            
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {          
            Creator.CreateFromArchetype(archetype.second->Name());
            break;
          }
        }
        ImGui::TreePop();
      }


      auto banks = Daisy->getSystem<Content>()->AllBanks();
      Resources.DisplayResourceList<BankMap>("Bank", banks);

      auto spriteLayers = Daisy->getSystem<Content>()->AllSpriteLayers();
      Resources.DisplayResourceList<SpriteLayerMap>("SpriteLayer", spriteLayers);

      if (ImGui::TreeNode("SpriteLayerOrder")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllSpriteLayerOrders()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            Select(resource.second.get());
            Windows.PropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Edit the selected SpriteLayerOrder
            DCTrace << "Editor::WidgetLibrary - Editing: " << resource.second->Name() << "\n";
            SelectedSpriteLayerOrder = resource.second;
            Windows.SpriteLayerOrderEditorEnabled = true;
            break;
          }

        }
        ImGui::TreePop();
      }


      auto spriteSources = Daisy->getSystem<Content>()->AllSpriteSources();
      Resources.DisplayResourceList<SpriteSourceMap>("SpriteSource", spriteSources);
      
      auto fonts = Daisy->getSystem<Content>()->AllFonts();
      Resources.DisplayResourceList<FontMap>("Font", fonts);
      
      auto soundCues = Daisy->getSystem<Content>()->AllSoundCues();
      Resources.DisplayResourceList<SoundCueMap>("SoundCue", soundCues);
      
      // Open the shader file perhaps?
      if (ImGui::TreeNode("Shader")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& shader : *Daisy->getSystem<Content>()->AllShaders()) {
          if (ImGui::Selectable(shader.second->Name().c_str())) {
            Select(shader.second.get());
            Windows.PropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      // Display every level
      if (ImGui::TreeNode("Level")) {
        for (auto& level : *Daisy->getSystem<Content>()->AllLevels()) {
          auto levelName = level.second->Name().c_str();
          if (ImGui::Selectable(levelName) ) {
            Select(level.second.get());
            Windows.PropertiesEnabled = true;
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

      auto collisionGroups = Daisy->getSystem<Content>()->AllCollisionGroups();
      Resources.DisplayResourceList<CollisionGroupMap>("CollisionGroup", collisionGroups);

      if (ImGui::TreeNode("CollisionTable")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *Daisy->getSystem<Content>()->AllCollisionTables()) {
          if (ImGui::Selectable(resource.second->Name().c_str())) {
            Select(resource.second.get());
            Windows.PropertiesEnabled = true;
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Edit the selected collision table
            DCTrace << "Editor::WidgetLibrary - Editing: " << resource.second->Name() << "\n";
            SelectedCollisionTable = resource.second.get();
            Windows.CollisionTableEditorEnabled = true;
            break;
          }

        }
        ImGui::TreePop();
      }

      auto physicsMaterials = Daisy->getSystem<Content>()->AllPhysicsMaterials();
      Resources.DisplayResourceList<PhysicsMaterialMap>("PhysicsMaterial", physicsMaterials);
      
      if (ImGui::TreeNode("ZilchScript")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& script : *Daisy->getSystem<Content>()->AllZilchScripts()) {
          if (ImGui::Selectable(script.second->Name().c_str())) {
            Select(script.second.get());
            Windows.PropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      ImGui::End();
    }



  }
}
