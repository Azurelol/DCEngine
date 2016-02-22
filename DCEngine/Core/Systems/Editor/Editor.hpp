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

    /**************************************************************************/
    /*!
    @brief Sets the property of a Resource through Zilch.
    @param property A pointer to the property.
    @param value The value which to set.
    */
    /**************************************************************************/
    template<typename PropertyType>
    inline void Editor::Set(Zilch::ExecutableState* state, ObjectPtr object, Zilch::Property * property, PropertyType value)
    {
      Zilch::ExceptionReport report;
      Zilch::Call setCall(property->Set, state);
      setCall.SetHandleVirtual(Zilch::Call::This, object);
      setCall.Set(0, value);
      setCall.Invoke(report);
    }

    /**************************************************************************/
    /*!
    @brief Checks if a property has been set as unsigned, and if it is,
           make sure the value can not be set negative.
    @param property A pointer to the property.
    @param value The value which to set.
    */
    /**************************************************************************/
    template<typename PropertyType>
    void Editor::CheckUnsigned(Zilch::Property * property, PropertyType & value)
    {
      if (!property->HasAttribute("Unsigned"))
        return;

      if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer))) {
        if (value < 0)
          value = 0;
      }
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real))) {
        if (value < 0.0f)
          value = 0.0f;
      }
      if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer4))) {
        if (value < 0)
          value = 0;
      }

    }

    /**************************************************************************/
    /*!
    @brief Selects a resource type, displaying every resource on the project.
    @param resourceType The name of the type of the resource.
    @param map A pointer to the map of the resources of the type.
    @param resource A pointer to the property.
    @param component A pointer to the component.
    @param propertyID Used for identifying for the GUI purposes.
    @return True if the resource was modified, false if otherwise.
    */
    /**************************************************************************/
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