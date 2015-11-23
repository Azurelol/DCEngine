/******************************************************************************/
/*!
@file   MainMenu.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/02/2015
@brief  This file includes the implementation for the Editor's library widget.
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
      
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Library", &WidgetLibraryEnabled);

      // 1. Display every spritesource
      if (ImGui::TreeNode("SpriteSource")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Sprites: ");
        for (auto spriteSrc : *Daisy->getSystem<Content>()->AllSpriteSources()) {
          ImGui::Text(spriteSrc.second->Name().c_str());
        }
        ImGui::TreePop();
      }
      // 2. Display every soundcue
      if (ImGui::TreeNode("SoundCue")) {        
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "SoundCues: ");
        for (auto soundCue : *Daisy->getSystem<Content>()->AllSoundCues()) {
          ImGui::Text(soundCue.second->Name().c_str());
        }
        ImGui::TreePop();
      }
      // 3. Display every shader
      if (ImGui::TreeNode("Shader")) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto shader : *Daisy->getSystem<Content>()->AllShaders()) {
          ImGui::Text(shader.second->Name().c_str());
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
        
        for (auto level : *Daisy->getSystem<Content>()->AllLevels()) {
          // If the user double-clicks on.. @todo not working yet
          if (ImGui::Selectable(level.second->Name().c_str())) {

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
