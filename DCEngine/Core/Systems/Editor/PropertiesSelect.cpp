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
    void SelectSpriteSource(Zilch::Property * resource, ObjectPtr component, unsigned int);
    void SelectSoundCue(Zilch::Property * resource, ObjectPtr component, unsigned int);
    void SelectLevel(Zilch::Property * resource, ObjectPtr component, unsigned int);

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the resource for the property.
    @param  resource A pointer to the BoundType of the resource.
    @param  object A pointer to the object that holds the resource.
    */
    /**************************************************************************/
    bool Editor::SelectResource(Zilch::Property * resource, ObjectPtr object, unsigned int& propertyID)
    {
      // Get the type of the resource      
      auto resourceType = std::string(resource->Name.c_str());
      ImGui::Text(resourceType.c_str()); 
      ImGui::PushID(propertyID++);
      // Whether a property was modified
      bool modified = false;

      if (resource->HasAttribute("Archetype")) {
        auto container = Daisy->getSystem<Content>()->AllArchetypes();
        modified = SelectResource<ArchetypeMap>("Archetype", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("SpriteSource")) {
        auto container = Daisy->getSystem<Content>()->AllSpriteSources();
        modified = SelectResource<SpriteSourceMap>("SpriteSource", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("SpriteLayer")) {
        auto container = Daisy->getSystem<Content>()->AllSpriteLayers();
        modified = SelectResource<SpriteLayerMap>("SpriteLayer", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("SpriteLayerOrder")) {
        auto container = Daisy->getSystem<Content>()->AllSpriteLayerOrders();
        modified = SelectResource<SpriteLayerOrderMap>("SpriteLayerOrder", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("Font")) {
        auto container = Daisy->getSystem<Content>()->AllFonts();
        modified = SelectResource<FontMap>("Font", container, resource, object, propertyID);
        //SelectSpriteSource(resource, object, propertyID);
      }
      else if (resource->HasAttribute("SoundCue")) {
        auto container = Daisy->getSystem<Content>()->AllSoundCues();
        modified = SelectResource<SoundCueMap>("SoundCue", container, resource, object, propertyID);
        //SelectSoundCue(resource, object, propertyID);
      }
      else if (resource->HasAttribute("Level")) {
        auto container = Daisy->getSystem<Content>()->AllLevels();
        modified = SelectResource<LevelMap>("Level", container, resource, object, propertyID);
        //SelectLevel(resource, object, propertyID);
      }
      else if (resource->HasAttribute("PhysicsMaterial")) {
        auto container = Daisy->getSystem<Content>()->AllPhysicsMaterials();
        modified = SelectResource<PhysicsMaterialMap>("PhysicsMaterial", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("CollisionGroup")) {
        auto container = Daisy->getSystem<Content>()->AllCollisionGroups();
        modified = SelectResource<CollisionGroupMap>("CollisionGroup", container, resource, object, propertyID);
      }
      else if (resource->HasAttribute("CollisionTable")) {
        auto container = Daisy->getSystem<Content>()->AllCollisionTables();
        modified = SelectResource<CollisionTableMap>("CollisionTable", container, resource, object, propertyID);
      }
      ImGui::PopID();

      return modified;
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the SpriteSource for the component.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectSpriteSource(Zilch::Property * resource, ObjectPtr component, unsigned int propertyID) {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSpriteSources();
      std::vector<const char *> spriteSourceNames;

      
      auto resourceValue = Reflection::PropertyAsString(resource, component);
      static int currentItem = 0;
      for (auto& spriteSource : *container) {
        // Push the name of it into the vector of strings
        spriteSourceNames.push_back(spriteSource.second->Name().c_str());
        if (spriteSource.second->getObjectName() == resourceValue)
          currentItem = static_cast<int>(spriteSourceNames.size()) - 1;
      }
      // Start at the current item
            
      // If the user selects an item... 
      if (ImGui::Combo("##propertyID", &currentItem, spriteSourceNames.data(), spriteSourceNames.size())) {
        // Set the selected item as the current resource
        auto selectedSpriteSource = spriteSourceNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->GetState());
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
    void SelectSoundCue(Zilch::Property * resource, ObjectPtr component, unsigned int propertyID)
    {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllSoundCues();
      std::vector<const char *> soundCueNames;

      // Create an exception report object
      Zilch::ExceptionReport report;
      // Grab the current property
      Zilch::Call getCall(resource->Get, Daisy->getSystem<Reflection>()->Handler()->GetState());
      getCall.SetHandleVirtual(Zilch::Call::This, component);
      getCall.Invoke(report);
      auto resourceValue = std::string(getCall.Get<Zilch::String>(Zilch::Call::Return).c_str());

      // Start at the current item
      static int currentItem = 0;
      for (auto soundCue : *container) {
        // Push the name of it into the vector of strings
        soundCueNames.push_back(soundCue.second->Name().c_str());
        if (soundCue.second->Name() == resourceValue)
          currentItem = static_cast<int>(soundCueNames.size()) - 1;
      }

      // If the user selects an item... 
      if (ImGui::Combo("##propertyID", &currentItem, soundCueNames.data(), static_cast<int>(soundCueNames.size()))) {
        // Set the selected item as the current resource
        auto selectedSoundCue = soundCueNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->GetState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedSoundCue));
        setCall.Invoke(report);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Allows the user to select the Level for the component property.
    @param  resource A pointer to the BoundType of the resource.
    @param  component A pointer to the component that holds the resource.
    */
    /**************************************************************************/
    void SelectLevel(Zilch::Property * resource, ObjectPtr component, unsigned int propertyID)
    {
      // Get a container of all active spritesources        
      auto container = Daisy->getSystem<Content>()->AllLevels();
      std::vector<const char *> levelNames;

      auto resourceValue = Reflection::PropertyAsString(resource, component);
      static int currentItem = 0;
      for (auto level : *container) {
        // Push the name of it into the vector of strings
        levelNames.push_back(level.second->Name().c_str());
        // Start at the current item
        if (level.second->Name() == resourceValue)
          currentItem = static_cast<int>(levelNames.size()) - 1;
      }

      // If the user selects an item... 
      if (ImGui::Combo("##propertyID", &currentItem, levelNames.data(), static_cast<int>(levelNames.size()))) {
        // Set the selected item as the current resource
        auto selectedLevel = levelNames.at(currentItem);
        Zilch::ExceptionReport report;
        Zilch::Call setCall(resource->Set, Daisy->getSystem<Reflection>()->Handler()->GetState());
        setCall.SetHandleVirtual(Zilch::Call::This, component);
        setCall.Set(0, Zilch::String(selectedLevel));
        setCall.Invoke(report);
      }
    }

  }
}
