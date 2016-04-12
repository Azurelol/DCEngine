/******************************************************************************/
/*!
\file   EditorSetup.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/21/2015
\brief  This file contains the functions which set up the Editor when it's
        toggled
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
/******************************************************************************/
#include "Editor.h"
// Engine
#include "../../Engine/Engine.h"
#include "../../Components/Camera.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Draws a grid on screen while the editor is enabled.
    */
    /**************************************************************************/
    void Editor::DrawGrid()
    {
      if (!Settings.GridActive)
        return;

      if (!EditorCamera)
        return;

      auto cameraPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
      // The editor grid will always be in front of the camera.
      Vec3 gridStartPos(cameraPos.x, cameraPos.y, 0);
      Real edge = 500; // We want to make sure we draw 'very' from far away??
      unsigned lines = 100;
      // Start drawings from the center of the screen and outwards
      for (unsigned int i = 0; i < lines; ++i) {
        // Draw the horizontal lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - edge, gridStartPos.y + (i * Settings.GridLength), gridStartPos.z),
          Vec3(gridStartPos.x + edge, gridStartPos.y + (i * Settings.GridLength), gridStartPos.z),
          Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - edge, gridStartPos.y - (i * Settings.GridLength), gridStartPos.z),
          Vec3(gridStartPos.x + edge, gridStartPos.y - (i * Settings.GridLength), gridStartPos.z),
          Settings.GridColor);
        // Draw the vertical lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x + (i * Settings.GridLength), gridStartPos.y - edge, gridStartPos.z),
          Vec3(gridStartPos.x + (i * Settings.GridLength), gridStartPos.y + edge, gridStartPos.z),
          Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - (i * Settings.GridLength), gridStartPos.y - edge, gridStartPos.z),
          Vec3(gridStartPos.x - (i * Settings.GridLength), gridStartPos.y + edge, gridStartPos.z),
          Settings.GridColor);
      }
    }

    /**************************************************************************/
    /*!
    @brief Toggles lightning on and off.
    */
    /**************************************************************************/
    void Editor::ToggleLightning()
    {

    }

    /**************************************************************************/
    /*!
    @brief Resizes the size of the rendering window while in editor mode.
    */
    /**************************************************************************/
    void Editor::ApplyEditorWindowLayout()
    {
      auto viewportResize = new Events::WindowResize();
      if (Active) {
        viewportResize->Dimensions.x = 1;
        viewportResize->Dimensions.y = 1;
      }
      else {
        viewportResize->Dimensions.x = Settings.ViewportResize.x;
        viewportResize->Dimensions.y = Settings.ViewportResize.y;
      }
      Daisy->Dispatch<Events::WindowResize>(viewportResize);
      delete viewportResize;
    }

    /**************************************************************************/
    /*!
    @brief Updates the current window's caption!
    */
    /**************************************************************************/
    void Editor::UpdateCaption()
    {
      // Get the current project's name
      auto projectName = Settings.ProjectProperties->ProjectName;
      std::string levelName;
      if (CurrentSpace->getCurrentLevel())
        levelName = CurrentSpace->getCurrentLevel()->getObjectName();

      DispatchSystemEvents::SetWindowCaption(projectName + " - Level: " + levelName + " - Daisy Chain Engine");
    }



  }
}