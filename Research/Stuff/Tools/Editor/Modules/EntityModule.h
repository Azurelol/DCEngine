/******************************************************************************/
/*
  File:    EntityModule.h
  Author:  Alejandro Hitti
  Date:    11/21/14
  Brief:   The Entity Module handles the creation of new entities and archetypes.
           It also holds a list of objects currently in the scene.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

namespace Editor
{
  class EntityModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static EntityModule* GetInstance(void)
    {
      static EntityModule instance;
      return &instance;
    }

    struct SortBy
    {
      enum SortByEnum
      {
        Search,
        Archetype,
        Name,
        NumSorts
      };
    };

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return EntityModuleInitialize(); }
    virtual bool IsActive()             override { return EntityModuleIsActive();   }
    virtual void SetActive(bool state)  override { EntityModuleSetActive(state);    }
    virtual bool IsVisible()            override { return EntityModuleIsVisible();  }
    virtual void SetVisible(bool state) override { EntityModuleSetVisible(state);   }
    virtual void Update()               override { EntityModuleUpdate();            }

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    EntityModule(void)                           = default;
    EntityModule(EntityModule&)                  = delete;
    EntityModule& operator=(const EntityModule&) = delete;
    ~EntityModule(void)                          = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/

    // Initializes the Module
    static bool EntityModuleInitialize();
    // Checks if the module is currently active
    static bool EntityModuleIsActive();
    // Sets the module active flag to the state passed in
    static void EntityModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool EntityModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void EntityModuleSetVisible(bool state);
    // Updates the module
    static void EntityModuleUpdate();

    // ------------------------------------/
    //           Private functions
    // ------------------------------------/

    // Calls the proper sort method
    static void SortObjectsBy(SortBy::SortByEnum sortType);
    // Organizes the objects list by archetype name
    static void SortByArchetype(void);
    // Organizes the objects list alphabetically
    static void SortByName(void);
    // Does object searching within the objects list
    static void SearchObjectsList(void);
    // Makes the buttons for the objects
    static void MakeObjectButtons(std::vector<Core::Entity*>& entities, int i);
    // Creates a new empty entity (no components)
    static void CreateEmptyEntity(void);
    // Creates a new entity from an archetype
    static void CreateEntityFromArchetype(void);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;
  };
}
