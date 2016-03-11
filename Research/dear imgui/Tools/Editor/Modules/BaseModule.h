/******************************************************************************/
/*
  File:    BaseModule.h
  Author:  Alejandro Hitti
  Date:    11/19/14
  Brief:   Base abstract module class that defines the basic functionality of
           every other editor module.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "ImGUI\imgui.h"
#include "EditorManager.h"

namespace Editor
{
  class BaseModule
  {
  public:
    // Defined so that every module calls it's own destructur
    virtual ~BaseModule() {}

    // Virtual functions inherited by every other module. Pure abstract class.
    virtual bool Initialize()           = 0;
    virtual bool IsActive()             = 0;
    virtual void SetActive(bool state)  = 0;
    virtual bool IsVisible()            = 0;
    virtual void SetVisible(bool state) = 0;
    virtual void Update()               = 0;
    //virtual void Uninitialize()         = 0;

  protected:
    static void SetHelpTooltip(std::string helpText)
    {
      if (ImGui::IsHovered() && EditorManager::GetTooltipState())
        ImGui::SetTooltip(helpText.c_str());
    }

  private:
    // Empty
  };
}
