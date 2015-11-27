/******************************************************************************/
/*!
@file      PropertiesSelect.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      11/02/2015
@brief     This file includes the implementation for the Editor's properties widget.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../../ComponentsInclude.h"
#include "../../Systems/Reflection/ZilchInterface.h"

namespace DCEngine {
  namespace Systems {

    // Resources
    void SelectSpriteSource(Zilch::Property * resource, ObjectPtr component);
    void SelectSoundCue(Zilch::Property * resource, ObjectPtr component);

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the resource for the property.
    @param  resource A pointer to the BoundType of the resource.
    @param  object A pointer to the object that holds the resource.
    */
    /**************************************************************************/
    void Editor::SelectResource(Zilch::Property * resource, ObjectPtr object)
    {
      // Get the type of the resource      
      auto resourceType = std::string(resource->Name.c_str());
      ImGui::Text(resourceType.c_str());
      // SpriteSource 
      if (resourceType == std::string("SpriteSource")) {
        SelectSpriteSource(resource, object);
      }
      // SoundCue
      else if (resourceType == std::string("SoundCue")) {
        SelectSoundCue(resource, object);
      }

    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the SpriteSource for the component.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectSpriteSource(Zilch::Property * resource, ObjectPtr component) {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSpriteSources();

      std::vector<const char *> spriteSourceNames;
      for (auto spriteSource : *container) {
        // Push the name of it into the vector of strings
        spriteSourceNames.push_back(spriteSource.second->Name().c_str());
      }
      // Start at the current item
      static int currentItem = 0;
      // If the user selects an item... 
      if (ImGui::Combo("##spritenames", &currentItem, spriteSourceNames.data(), spriteSourceNames.size())) {
        // Set the selected item as the current resource
        auto selectedSpriteSource = spriteSourceNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedSpriteSource));
        setCall.Invoke(report);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the SoundCue for the component.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectSoundCue(Zilch::Property * resource, ObjectPtr component)
    {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSoundCues();
      std::vector<const char *> soundCueNames;
      for (auto soundCue : *container) {
        // Push the name of it into the vector of strings
        soundCueNames.push_back(soundCue.second->Name().c_str());
      }
      // Start at the current item
      static int currentItem = 0;
      // If the user selects an item... 
      if (ImGui::Combo("##soundCueNames", &currentItem, soundCueNames.data(), soundCueNames.size())) {
        // Set the selected item as the current resource
        auto selectedSoundCue = soundCueNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->getState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedSoundCue));
        setCall.Invoke(report);
      }
    }

  }
}
