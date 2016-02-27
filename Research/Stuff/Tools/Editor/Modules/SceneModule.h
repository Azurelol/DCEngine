/******************************************************************************/
/*
  File:    SceneModule.h
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Scene Module is in charge of managing the scene and the levels.
           It'll let you create, save and load levels.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

namespace Editor
{
  class SceneModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static SceneModule* GetInstance(void)
    {
      static SceneModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return SceneModuleInitialize(); }
    virtual bool IsActive()             override { return SceneModuleIsActive();   }
    virtual void SetActive(bool state)  override { SceneModuleSetActive(state);    }
    virtual bool IsVisible()            override { return SceneModuleIsVisible();  }
    virtual void SetVisible(bool state) override { SceneModuleSetVisible(state);   }
    virtual void Update()               override { SceneModuleUpdate();            }

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    SceneModule(void)                          = default;
    SceneModule(SceneModule&)                  = delete;
    SceneModule& operator=(const SceneModule&) = delete;
    ~SceneModule(void)                         = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/
    
    // Initializes the Module
    static bool SceneModuleInitialize();
    // Checks if the module is currently active
    static bool SceneModuleIsActive();
    // Sets the module active flag to the state passed in
    static void SceneModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool SceneModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void SceneModuleSetVisible(bool state);
    // Updates the module
    static void SceneModuleUpdate();

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    static char* m_SceneName;

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;
  };
}
