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



  }
}
