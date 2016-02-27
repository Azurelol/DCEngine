/******************************************************************************/
/*
  File:    ResourcesModule.h
  Author:  Alejandro Hitti
  Date:    02/07/15
  Brief:   The Resources Module handles all in-game Resources, such as textures,
           models, archetypes, etc.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

// Function signature to edit a resource
typedef void(*EditFnSig)(std::vector<std::string>& resources, int index);

namespace Editor
{
  class ResourcesModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static ResourcesModule* GetInstance(void)
    {
      static ResourcesModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return ResourcesModuleInitialize(); }
    virtual bool IsActive()             override { return ResourcesModuleIsActive();   }
    virtual void SetActive(bool state)  override { ResourcesModuleSetActive(state);    }
    virtual bool IsVisible()            override { return ResourcesModuleIsVisible();  }
    virtual void SetVisible(bool state) override { ResourcesModuleSetVisible(state);   }
    virtual void Update()               override { ResourcesModuleUpdate();            }

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    ResourcesModule(void)                              = default;
    ResourcesModule(ResourcesModule&)                  = delete;
    ResourcesModule& operator=(const ResourcesModule&) = delete;
    virtual ~ResourcesModule(void);

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/

    // Initializes the Module
    static bool ResourcesModuleInitialize();
    // Checks if the module is currently active
    static bool ResourcesModuleIsActive();
    // Sets the module active flag to the state passed in
    static void ResourcesModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool ResourcesModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void ResourcesModuleSetVisible(bool state);
    // Updates the module
    static void ResourcesModuleUpdate();

    // ------------------------------------/
    //           Private functions
    // ------------------------------------/
    static void DisplayFilterList(std::vector<std::string>& resourceNames, EditFnSig editFn, bool hasPreview);

    // Edit functions per type of resource
    static void EditModel(std::vector<std::string>& resources, int index);
    static void EditTexture(std::vector<std::string>& resources, int index);
    static void EditArchetype(std::vector<std::string>& resources, int index);

    // Checks if the archetype name is unique
    static bool IsArchetypeNameUnique(std::string& archName);

    // Displays the Model/Texture/Archetype tabs in the resources module
    static void DisplayModelResources(void);
    static void DisplayTextureResources(void);
    static void DisplayArchetypeResources(void);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;
    static bool m_IsFirstTime;

    static Core::Entity* m_PreviewTexture;
    static Core::Entity* m_ArchetypeEdit;
  };
}
