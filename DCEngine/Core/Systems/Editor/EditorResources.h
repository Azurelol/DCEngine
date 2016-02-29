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

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorResources {
    public:
      EditorResources(Editor& editor);
      template <typename ResourceMap> bool DisplayResourceList(std::string type, ResourceMap* map);
      // Templated genius function
      //template <typename ResourceMap, typename SingleClickFn, typename DoubleClickFn, typename ...Args> 
      //bool DisplayResourceList(std::string type, ResourceMap* map, 
      //                          void(SingleClickFn::* singleClickFn)(Args...), 
      //                          void(DoubleClickFn::* doubleClickFn)(Args...)) {
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
      Editor& EditorRef;

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
          bool selected = EditorRef.SelectedObject() && EditorRef.SelectedObject()->getObjectID() == resource.second->getObjectID();
          if (ImGui::Selectable(resourceName, selected) ) {
            EditorRef.Select(resource.second.get());
            EditorRef.Inspector.Toggle(true);
          }
        }
        ImGui::TreePop();
      }
      return true;
    }

  }
}