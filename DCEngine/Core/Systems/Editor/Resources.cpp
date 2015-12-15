/******************************************************************************/
/*!
@file   MenuResources.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/15/2015
@brief  This file includes the implementation for the Editor's resource menu.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {




    /**************************************************************************/
    /*!
    @brief  Displays the 'ResourceCreate' editor window.
    */
    /**************************************************************************/
    void Editor::WindowAddResource()
    {
      if (!WindowAddResourceEnabled)
        return;
            
        ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Begin("Add a Resource", &WindowAddResourceEnabled);

        static char resourceName[32];
        // Resource Selection
        static ResourceType type = ResourceType::None;
        if (ImGui::Selectable("CollisionGroup")) {
          type = ResourceType::CollisionGroup;
          strcpy(resourceName, "NewCollisionGroup");
        }
        if (ImGui::Selectable("CollisionTable")) {
          type = ResourceType::CollisionTable;
          strcpy(resourceName, "NewCollisionTable");
        }
        if (ImGui::Selectable("Level")) {
          type = ResourceType::Level;
          strcpy(resourceName, "NewLevel");
        }
        if (ImGui::Selectable("Material")) {
          type = ResourceType::Material;
          strcpy(resourceName, "NewMaterial");
        }
        if (ImGui::Selectable("PhysicsMaterial")) {
          type = ResourceType::PhysicsMaterial;
          strcpy(resourceName, "NewPhysicsMaterial");
        }
        if (ImGui::Selectable("SoundCue")) {
          type = ResourceType::SoundCue;
          strcpy(resourceName, "NewSoundCue");
        }
        if (ImGui::Selectable("SpriteSource")) {
          type = ResourceType::SpriteSource;
          strcpy(resourceName, "NewSpriteSource");
        }
        if (ImGui::Selectable("SpriteLayer")) {
          type = ResourceType::SpriteLayer;
          strcpy(resourceName, "NewSpriteLayer");
        }
        ImGui::Separator();
        // Name
        ImGui::InputText("Name:", resourceName, IM_ARRAYSIZE(resourceName));
        // Buttons
        ImGui::Separator();
        if (ImGui::Button("Create")) {
          if (type == ResourceType::SpriteSource || type == ResourceType::SoundCue)
            DCTrace << "Editor::WindowAddResource - That resource cannot be created without an asset file! \n";
          else
            ResourceCreate(std::string(resourceName), type);
        }
        ImGui::SameLine();
        if (ImGui::Button("From File")) {
          if (type == ResourceType::SpriteSource || type == ResourceType::SoundCue)
            ResourceAddFromFile(std::string(resourceName), type);
          else
            DCTrace << "Editor::WindowAddResource - That resource cannot be added from file! \n";
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          WindowAddResourceEnabled = false;
        }

        ImGui::End();
      
    }

    /**************************************************************************/
    /*!
    @brief Loads the hard-coded level, Dollhouse.
    */
    /**************************************************************************/
    void Editor::LoadDollhouse()
    {
      SelectedObject = nullptr;
      // !!! TESTING: Level loading
      LevelPtr dollhouse = LevelPtr(new DollHouse(*CurrentSpace, *CurrentSpace->getGameSession()));
      CurrentSpace->LoadSampleLevel(dollhouse);      
      // Load the editor camera
      SetEditorCamera(true);
    }

  }
}