/******************************************************************************/
/*
  File:    MainModule.h
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The Main Module is in charge of activating and managing all the other
           modules in the GUI Interface.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

namespace Editor
{
  class MainModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static MainModule* GetInstance(void)
    {
      static MainModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return MainModuleInitialize(); }
    virtual bool IsActive()             override { return MainModuleIsActive();   }
    virtual void SetActive(bool state)  override { MainModuleSetActive(state);    }
    virtual bool IsVisible()            override { return MainModuleIsVisible();  }
    virtual void SetVisible(bool state) override { MainModuleSetVisible(state);   }
    virtual void Update()               override { MainModuleUpdate();            }

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    MainModule(void)                         = default;
    MainModule(MainModule&)                  = delete;
    MainModule& operator=(const MainModule&) = delete;
    ~MainModule(void)                        = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/
    
    // Initializes the Module
    static bool MainModuleInitialize();
    // Checks if the module is currently active
    static bool MainModuleIsActive();
    // Sets the module active flag to the state passed in
    static void MainModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool MainModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void MainModuleSetVisible(bool state);
    // Updates the module
    static void MainModuleUpdate();

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;
  };
}
