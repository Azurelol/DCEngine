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
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetLibrary()
    {
      if (!WidgetLibraryEnabled)
        return;
      
      bool clicked;      
      if (clicked = ImGui::IsMouseDoubleClicked(0)) {
        DCTrace << "Double clicked checck B \n";
      }
      //if (clicked = ImGui::GetIO().MouseDoubleClicked[0]) {
      //  DCTrace << "Double clicked checck A \n";
      //}

      //if (ImGui::GetIO().MouseDoubleClicked[1]) {
      //  DCTrace << "Double clicked!! \n";
      //}
      //if (ImGui::GetIO().MouseDoubleClicked[2]) {
      //  DCTrace << "Double clicked!! \n";
      //}

      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Library", &WidgetLibraryEnabled);

      // Archetype Tree
      if (ImGui::TreeNode("Archetype")) {
        // Query the Content system for the current list of archetypes
        static bool ScannedForArchetypes;
        if (!ScannedForArchetypes) {
          Daisy->getSystem<Content>()->ScanForArchetypes();
          ScannedForArchetypes = true;
        }
        
        // Display all archetypes.
        for (auto& archetype : *Daisy->getSystem<Content>()->AllArchetypes()) {
          if (ImGui::Selectable(archetype.second->Name().c_str())) {
            SelectedObject = archetype.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }

      // SpriteSource
      if (ImGui::TreeNode("SpriteSource")) {
        // Display every SpriteSource
        for (auto& spriteSrc : *Daisy->getSystem<Content>()->AllSpriteSources()) {
          if (ImGui::Selectable(spriteSrc.second->Name().c_str())) {    
            
            //auto a = ImGui::GetID(spriteSrc.second->Name().c_str());            
            //if (ImGui::IsMouseDoubleClicked(a)) {
            //  DCTrace << "hey ";
            //}
            SelectedObject = spriteSrc.second.get();
            WindowPropertiesEnabled = true;
          }
        }
        ImGui::TreePop();
      }
      // 2. Display every soundcue
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
      // 3. Display every shader
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
        // Query the Content system for the current list of levels
        static bool ScannedForLevels;
        if (!ScannedForLevels) {
          Daisy->getSystem<Content>()->ScanForLevels();
          ScannedForLevels = true;
        }
        
        for (auto& level : *Daisy->getSystem<Content>()->AllLevels()) {
          // If the user double-clicks on.. @todo not working yet


          if (ImGui::Selectable(level.second->Name().c_str()) ) {
            
            if (ImGui::IsMouseDoubleClicked(0)) {
              DCTrace << "hey\n";
            }

            SelectedObject = level.second.get();
            WindowPropertiesEnabled = true;
          }
          
        }
        
        //ScannedForLevels = false;
        ImGui::TreePop();
      }

      ImGui::End();



      //ImGui::SetNextWindowSize(ImVec2(200, 500), ImGuiSetCond_FirstUseEver);
      //// Title
      //ImGui::Begin("Library", &EditorEnabled);
      //// Print every name
      //auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      //for (auto object : *objects) {
      //  ImGui::Text(object->Name().c_str());
      //}
      //// Ends the window
      //ImGui::End();
    }



  }
}
