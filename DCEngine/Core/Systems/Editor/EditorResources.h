/******************************************************************************/
/*!
@file   EditorResources.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Objects/Resource.h"
#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorResources : public EditorModule {
    public:
      
      void ToggleDuplicateLevel();

      // CTOR
      EditorResources();
      template <typename ResourceMap> bool DisplayResourceList(std::string type, ResourceMap* map);      
      template <typename ResourceMap>
      bool DisplayResourceList(std::string resourceType, ResourceMap* map,
        std::function<void(const std::string&)> singleClick,
        std::function<void(const std::string&)> doubleClick);
      

    private:
      // Windows
      struct Windows {
        bool DuplicateLevels;
        Windows() : DuplicateLevels(false) {}
      };
      Windows ActiveWindows;
      // Methods
      void DisplayDuplicateLevelWindow();
      void DuplicateLevel(const std::string& name);

    };
    

    /**************************************************************************/
    /*!
    @brief Displays all the resources on the map as a list.
    @param type The name of the resource type.
    @param map A poiner to the map that holds the resources.
    */
    /**************************************************************************/
    template<typename ResourceMap>
    inline bool EditorResources::DisplayResourceList(std::string type, ResourceMap* map)
    {
      // If the resource map is empty...
      if (map->empty())
        return false;

      if (ImGui::TreeNode(type.c_str())) {
        //ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
        for (auto& resource : *map) {
          auto resourceName = resource.second->Name().c_str();
          bool selected = Access().SelectedObject() && Access().SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected) ) {
            Access().Select(resource.second.get());
            Access().Inspector.Toggle(true);
          }
        }
        ImGui::TreePop();
      }
      return true;
    }

    template<typename ResourceMap>
    inline bool EditorResources::DisplayResourceList(std::string resourceType, ResourceMap * map, std::function<void(const std::string&)> singleClick, std::function<void(const std::string&)> doubleClick)
    {
        if (ImGui::TreeNode(resourceType.c_str())) {
          for (auto& resource : *map) {
            auto resourceName = resource.second->Name().c_str();

            bool selected = Access().SelectedObject() && Access().SelectedObject()->getObjectID() 
                                                                   == resource.second->getObjectID();
            if (ImGui::Selectable(resourceName, selected)) {
              singleClick(resourceName);      
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
              doubleClick(resourceName);
              break;
            }
          }
          ImGui::TreePop();
        }
        return true;
    }

  }
}