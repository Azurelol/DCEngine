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
      
      auto archetypes = Daisy->getSystem<Content>()->AllArchetypes();
      //Resources.DisplayResourceList<ArchetypeMap>("Archetypes", archetypes, &Archetypes.Select, &Creator.CreateFromArchetype);
      if (ImGui::TreeNode("Archetype")) {
        for (auto& resource : *archetypes) {
          auto resourceName = resource.second->Name().c_str();
          auto current = Archetypes.Current();
          bool selected = current && current->Name() == resource.second->Name();
          if (ImGui::Selectable(resourceName, selected)) {
            Archetypes.Select(resource.second->Name());
            //Select(archetype.second.get());            
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {          
            Creator.CreateFromArchetype(resource.second->Name());
            break;
          }
        }
        ImGui::TreePop();
      }


      auto banks = Daisy->getSystem<Content>()->AllBanks();
      Resources.DisplayResourceList<BankMap>("Bank", banks);

      auto spriteLayers = Daisy->getSystem<Content>()->AllSpriteLayers();
      Resources.DisplayResourceList<SpriteLayerMap>("SpriteLayer", spriteLayers);

      auto spriteLayerOrders = Daisy->getSystem<Content>()->AllSpriteLayerOrders();
      if (ImGui::TreeNode("SpriteLayerOrder")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *spriteLayerOrders) {
          auto resourceName = resource.second->Name().c_str();
          bool selected = SelectedObject() && SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected)) {
            Select(resource.second.get());
            Inspector.Toggle(true);
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
      
      auto shaders = Daisy->getSystem<Content>()->AllShaders();
      Resources.DisplayResourceList<ShaderMap>("Shader", shaders);

      // Display every level
      if (ImGui::TreeNode("Level")) {
        for (auto& resource : *Daisy->getSystem<Content>()->AllLevels()) {
          auto resourceName = resource.second->Name().c_str();
          bool selected = SelectedObject() && SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected)) {
            Select(resource.second.get());
            Inspector.Toggle(true);
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // Load the selected level
            DCTrace << "Editor::WidgetLibrary - Loading: " << resourceName << "\n";
            LoadLevel(resourceName);
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
          auto resourceName = resource.second->Name().c_str();
          bool selected = SelectedObject() && SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected)) {
            Select(resource.second.get());
            Inspector.Toggle(true);
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
        for (auto& resource : *Daisy->getSystem<Content>()->AllZilchScripts()) {
          auto resourceName = resource.second->Name().c_str();
          bool selected = SelectedObject() && SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected)) {
            Select(resource.second.get());
          }
          if (GUI::IsMouseDoubleClicked()) {
            TextEditor.Load(resource.second);
          }
        }
        ImGui::TreePop();
      }

      ImGui::End();
    }



  }
}
