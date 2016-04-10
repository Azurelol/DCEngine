/******************************************************************************/
/*!
@file   EditorInspector.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EditorModule.h"

#include "../../Resources/ResourcesInclude.h"

namespace DCEngine {
  namespace Systems {
    class Editor;
    class EditorInspector : public EditorModule {
    public:

      void Display();
      EditorInspector();
      // Select
      void SelectSpace();
      void SelectGameSession();

    private:
      
      // Property Display
      bool DisplayProperties(ObjectPtr, Zilch::Handle = Zilch::Handle());
      void DisplayEntityProperties(EntityPtr);
      void DisplayResourceProperties(ResourcePtr);
      void DisplayAdditionalProperties(ResourcePtr resource);
      bool InspectResource(Zilch::Property * resource, ObjectPtr object, unsigned int& propertyID);
      // Previews
      void PreviewImage(Zilch::Property* property, ObjectPtr object);
      void PreviewSound(Zilch::Property* property, ObjectPtr object);
      // Components
      bool AddComponent(EntityPtr);
      bool CheckIfDoneModified(bool& modified);
      // Templates
      template <typename ResourceMap> bool InspectResource(std::string resourceType, ResourceMap* map, Zilch::Property * resource, ObjectPtr component, unsigned int propertyID);
      template <typename PropertyType> void Get(Zilch::ExecutableState* state, ObjectPtr object, Zilch::Handle handle, Zilch::Property* property, PropertyType value);
      template <typename PropertyType> void Set(Zilch::ExecutableState* state, ObjectPtr object, Zilch::Handle handle, Zilch::Property* property, PropertyType value);
      template <typename PropertyType> void CheckUnsigned(Zilch::Property* property, PropertyType& value);
      void OnEditorSaveEvent(Events::EditorSave* event);
    };

  }
}

#include "EditorInspector.hpp"