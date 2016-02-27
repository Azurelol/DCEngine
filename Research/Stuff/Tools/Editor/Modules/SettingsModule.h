/******************************************************************************/
/*
  File:    SettingsModule.h
  Author:  Alejandro Hitti
  Date:    11/25/14
  Brief:   The Settings Module lets the user modify certain aspects of the
           editor's functionality and appearance.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

namespace Editor
{
  namespace Styles
  {
    // Generates an enum with all the styles
    #define STYLE(x) x,
    enum Style
    {
      #include "StylesDeclaration.h"
      NumStyles
    };
    #undef STYLE

    extern const char* StyleStrings[NumStyles + 1];
  }

  class SettingsModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static SettingsModule* GetInstance(void)
    {
      static SettingsModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return SettingsModuleInitialize(); }
    virtual bool IsActive()             override { return SettingsModuleIsActive();   }
    virtual void SetActive(bool state)  override { SettingsModuleSetActive(state);    }
    virtual bool IsVisible()            override { return SettingsModuleIsVisible();  }
    virtual void SetVisible(bool state) override { SettingsModuleSetVisible(state);   }
    virtual void Update()               override { SettingsModuleUpdate();            }

    // Changes the style to the specified one
    static void ChangeStyle(Styles::Style style);

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    SettingsModule(void)                             = default;
    SettingsModule(SettingsModule&)                  = delete;
    SettingsModule& operator=(const SettingsModule&) = delete;
    ~SettingsModule(void)                            = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/

    // Initializes the Module
    static bool SettingsModuleInitialize();
    // Checks if the module is currently active
    static bool SettingsModuleIsActive();
    // Sets the module active flag to the state passed in
    static void SettingsModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool SettingsModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void SettingsModuleSetVisible(bool state);
    // Updates the module
    static void SettingsModuleUpdate();

    // ------------------------------------/
    //           Private functions
    // ------------------------------------/

    // Provides an interface to change the sizing, padding, etc, of the editor
    static void ChangeSizes(void);
    // Changes window options to alter the editor
    static void ChangeWindowOptions(void);
    // Saves style into a file. If not found, selects default style.
    static void SaveStyle(void);
    // Displays the options collapsable menu
    static void ShowOptions(void);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    // Internal style that determines the appearance of the editor
    static ImGuiStyle m_Style;

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;
  };
}
