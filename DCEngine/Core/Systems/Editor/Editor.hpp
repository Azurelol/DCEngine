/******************************************************************************/
/*!
@file   Editor.hpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   1/24/2015
@brief  The file that holds all template functions used by the Editor.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
//#pragma once
#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    template<typename ResourceMap>
    bool Editor::SelectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID)
    {
      // Get a container of all active resources
      std::vector<const char *> resources;
      auto resourceValue = Reflection::PropertyAsString(resource, component);
      static int currentItem = 0;
      for (auto resource : *map) {
        // Push the name of it into the vector of strings
        resources.push_back(resource.second->Name().c_str());
        if (resource.second->getObjectName() == resourceValue)
          currentItem = static_cast<int>(resources.size()) - 1;
      }

      // If the user selects an item... 
      if (ImGui::Combo("##propertyID", &currentItem, resources.data(), static_cast<int>(resources.size()))) {
        // Set the selected item as the current resource
        auto selectedResource = resources.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedResource));
        setCall.Invoke(report);
        // Property was modified..
        return true;
      }
      // Property was not modified
      return false;
    }

  }
}