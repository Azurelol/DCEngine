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
      EditorResources();
      template <typename ResourceMap> bool DisplayResourceList(std::string type, ResourceMap* map);
      // Templated genius function
      template <typename ResourceMap>
      bool DisplayResourceList(std::string resourceType, ResourceMap* map,
        std::function<void(const std::string&)> singleClick,
        std::function<void(const std::string&)> doubleClick);

      //template <typename ResourceMap, typename SingleClickClass, typename DoubleClickClass,
      //          typename SingleClickFn, typename DoubleClickFn> 
      //bool DisplayResourceList(std::string type, ResourceMap* map, SingleClickClass scClass, 
      //                          void(SingleClickClass::* SingleClickFn singleClickFn)(const std::string&),
      //                          void(DoubleClickClass::* DoubleClickFn doubleClickFn)(const std::string&)) {
      //  if (ImGui::TreeNode(type.c_str())) {
      //    for (auto& resource : map) {
      //      auto resourceName = resource.second->Name().c_str();

      //      bool selected = Access().SelectedObject() && Access().SelectedObject()->getObjectID() 
      //                                                             == resource.second->getObjectID();
      //      if (ImGui::Selectable(resourceName, selected)) {
      //        SingleClickClass.SingleClickFn(resourceName);      
      //      }
      //      if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
      //        Creator.CreateFromArchetype(resource.second->Name());
      //        break;
      //      }
      //    }
      //    ImGui::TreePop();
      //  }
      //}

      //void WindowAddResource();
      //void ResourceCreate(std::string& name, ResourceType type);
      //void ResourceAddFromFile(std::string& name, ResourceType type);
      //bool CreateLevel(std::string&);
      //ResourcePtr CreateBank(std::string& name, std::string& assetPath);
      //ResourcePtr CreateCollisionGroup(std::string& name);
      //ResourcePtr CreateCollisionTable(std::string& name);
      //ResourcePtr CreatePhysicsMaterial(std::string& name);
      //ResourcePtr CreateSpriteSource(std::string& name, std::string& assetPath);
      //ResourcePtr CreateSoundCue(std::string& name, std::string& assetPath);
      //ResourcePtr CreateZilchScript(std::string& name);
      //ResourcePtr CreateFont(std::string& name, std::string& assetPath);
      //ResourcePtr CreateSpriteLayer(std::string& name);
      //ResourcePtr CreateSpriteLayerOrder(std::string& name);
      //bool SelectEnumeration(Zilch::Property*, ObjectPtr, unsigned int&);
      //bool SelectResource(Zilch::Property*, ObjectPtr, unsigned int&);
      //template <typename ResourceMap>
      //bool SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID);
      //void WindowCollisionTableEditor();
      //void WindowSpriteLayerOrderEditor();
      //ResourcePtr SelectedCollisionTable;
      //SpriteLayerOrderPtr SelectedSpriteLayerOrder;

    private:

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