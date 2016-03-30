/******************************************************************************/
/*
  File:    InspectorModule.h
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Inspector Module handles the inspection of all the components
           inside of the selected object.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

class Reflex::Variant;
class Reflex::Member;

namespace Editor
{
  class InspectorModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static InspectorModule* GetInstance(void)
    {
      static InspectorModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return InspectorModuleInitialize(); }
    virtual bool IsActive()             override { return InspectorModuleIsActive();   }
    virtual void SetActive(bool state)  override { InspectorModuleSetActive(state);    }
    virtual bool IsVisible()            override { return InspectorModuleIsVisible();  }
    virtual void SetVisible(bool state) override { InspectorModuleSetVisible(state);   }
    virtual void Update()               override { InspectorModuleUpdate();            }

    //
    // Made these functions public to use them in the Resources Module
    //

    // Inspects all the components from an entity. Returns true if a value has been changed, false otherwise.
    static bool Inspect(Core::Entity* entity);
    // Add/Remove components. Returns true if a component was added, false otherwise.
    static bool AddComponent(Core::Entity* entity);
    // Change entity name. Returns true if the name was changed, false otherwise.
    static bool ChangeName(Core::Entity* entity);

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    InspectorModule(void)                              = default;
    InspectorModule(InspectorModule&)                  = delete;
    InspectorModule& operator=(const InspectorModule&) = delete;
    ~InspectorModule(void)                             = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/
    
    // Initializes the Module
    static bool InspectorModuleInitialize();
    // Checks if the module is currently active
    static bool InspectorModuleIsActive();
    // Sets the module active flag to the state passed in
    static void InspectorModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool InspectorModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void InspectorModuleSetVisible(bool state);
    // Updates the module
    static void InspectorModuleUpdate();

    // ------------------------------------/
    //           Private functions
    // ------------------------------------/

    // Inspects all the components from an entity. Returns true if a value has been changed, false otherwise.
    static bool InspectRec(Reflex::Variant var, Core::Entity* entity = nullptr);
    static bool InspectPod(Reflex::Variant var, const Reflex::Member* mem, const Core::Entity* entity = nullptr);
    static bool IsBaseCase(const Reflex::TypeInfo* info);

    // Handles base cases for adding components
    static void AddComponentBaseCases(Core::Entity* entity, Core::Component* component);

    // Saves the current entity as an archetype
    static void SaveToArchetype(Core::Entity* entity);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    static char m_TextBuffer[256];

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;

    static std::vector<std::string> m_OrderedComponentStrings;
  };
}
