/******************************************************************************/
/*
  File:    ProfilerModule.h
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   The profiler module displays graphs and metrics about the performance
           of the game at runtime.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "BaseModule.h"

namespace Editor
{
  class ProfilerModule : public BaseModule
  {
  public:
    // Gettor for the singleton instance
    static ProfilerModule* GetInstance(void)
    {
      static ProfilerModule instance;
      return &instance;
    }

    // ------------------------------------/
    //           Virtual functions
    // ------------------------------------/

    // Adding static functions to the virtual ones inherited from BaseModule
    // allow us to make a container of modules that are implemented as singletons
    virtual bool Initialize()           override { return ProfilerModuleInitialize(); }
    virtual bool IsActive()             override { return ProfilerModuleIsActive();   }
    virtual void SetActive(bool state)  override { ProfilerModuleSetActive(state);    }
    virtual bool IsVisible()            override { return ProfilerModuleIsVisible();  }
    virtual void SetVisible(bool state) override { ProfilerModuleSetVisible(state);   }
    virtual void Update()               override { ProfilerModuleUpdate();            }

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    ProfilerModule(void)                             = default;
    ProfilerModule(ProfilerModule&)                  = delete;
    ProfilerModule& operator=(const ProfilerModule&) = delete;
    ~ProfilerModule(void)                            = default;

    // ------------------------------------/
    //           Static functions
    // ------------------------------------/
    
    // Initializes the Module
    static bool ProfilerModuleInitialize();
    // Checks if the module is currently active
    static bool ProfilerModuleIsActive();
    // Sets the module active flag to the state passed in
    static void ProfilerModuleSetActive(bool state);
    // Checks if the module is currently visible
    static bool ProfilerModuleIsVisible();
    // Sets the module visible flag to the state passed in
    static void ProfilerModuleSetVisible(bool state);
    // Updates the module
    static void ProfilerModuleUpdate();

    // ------------------------------------/
    //           Private functions
    // ------------------------------------/

    // Plots the systems elapsed time into a histogram
    static void PlotSystemsHistogram(float minTime, float maxTime);
    // Plots each system's line chart
    static void PlotSystemLineChart(float minTime, float maxTime);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/
    static bool m_IsActive;
    static bool m_IsVisible;

    static float* systemsData;
    static std::string systemsNames;

    // Used to set the position of the module inside of the window area
    static bool m_IsFirstTime;
  };
}
