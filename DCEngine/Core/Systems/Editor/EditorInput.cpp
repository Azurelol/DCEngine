/******************************************************************************/
/*!
@file   EditorInput.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/17/2015
@brief  This file includes the implementation for the Editor's input binding.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
/******************************************************************************/
#include "Editor.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Receives a MouseDown event.
    @param  event The MouseDown event.
    */
    /**************************************************************************/
    void Editor::OnMouseDownEvent(Events::MouseDown* event)
    {
      if (!EditorEnabled)
        return;

      if (event->ButtonPressed == MouseButton::Left) {
        // Look for an object that matches the translation
        auto posOnSpace = CurrentSpace->getComponent<CameraViewport>()->ScreenToViewport(event->Position);
        auto gameObject = FindObjectFromSpace(posOnSpace);

        // If the 'Select' tool is active, attempt to pick it
        if (ActiveTool == EditorTool::Select) {
          SelectObjectFromSpace(gameObject);
        }
        // If the 'Translate' tool is active...
        if (ActiveTool == EditorTool::Translate) {
          // And a valid GameObject was selected, start dragging it
          if (gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {
            Settings.Dragging = true;
            DCTrace << "Editor::OnMouseDownEvent - Dragging: '" << gameObject->getObjectName() << "'\n";
          }
        }
      }
      else if (event->ButtonPressed == MouseButton::Right) {
        Settings.Panning = true;        
      }
    }

    /**************************************************************************/
    /*!
    @brief  Receives a MouseUp event.
    @param  event The MouseUp event.
    */
    /**************************************************************************/
    void Editor::OnMouseUpEvent(Events::MouseUp* event)
    {
      if (!EditorEnabled)
        return;

      // Stop dragging
      if (Settings.Dragging) {
        ReleaseObject();
        Settings.Dragging = false;
      }
      // Stop panning
      if (Settings.Panning) {
        Settings.Panning = false;
      }

      //DCTrace << "Editor::OnMouseUpEvent - \n";
    }

    /**************************************************************************/
    /*!
    @brief  Receives a MouseUpdate event.
    @param  event The MouseUpdate event.
    */
    /**************************************************************************/
    void Editor::OnMouseUpdateEvent(Events::MouseUpdate * event)
    {
      DragObject(event->ScreenPosition);
      PanCamera(event->ScreenPosition);
    }


    /**************************************************************************/
    /*!
    @brief  Receives a KeyDown event.
    @param  event A pointer to the event.
    */
    /**************************************************************************/
    void Editor::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::Escape:
        ActiveTool = EditorTool::None;
        break;

      case Keys::Return:
        ActiveTool = EditorTool::None;
        break;

      case Keys::Tilde:
        WindowConsoleEnabled = !WindowConsoleEnabled;
        break;

      case Keys::F5:        
        PlayGame();
        break;

      case Keys::F8:
        StopGame();
        break;

      case Keys::Tab:
        WindowToolsEnabled = !WindowToolsEnabled;
        break;

      case Keys::Num1:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::SelectTool \n";
        ActiveTool = EditorTool::Select;
        break;

      case Keys::Num2:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::TranslateTool \n";
        ActiveTool = EditorTool::Translate;
        break;

      case Keys::Num3:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::RotateTool \n";
        ActiveTool = EditorTool::Rotate;
        break;

      case Keys::Num4:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::ScaleTool \n";
        ActiveTool = EditorTool::Scale;
        break;

      case Keys::Delete:
        DeleteObject();
        break;

      case Keys::Up:
        MoveObject(Vec3(0, Settings.SnapDistance, 0));
        ScaleObject(Vec3(0, Settings.SnapDistance, 0));
        break;

      case Keys::Down:
        MoveObject(Vec3(0, -Settings.SnapDistance, 0));
        ScaleObject(Vec3(0, -Settings.SnapDistance, 0));
        break;

      case Keys::Left:
        MoveObject(Vec3(-Settings.SnapDistance, 0,0));
        ScaleObject(Vec3(-Settings.SnapDistance, 0, 0));
        break;

      case Keys::Right:
        MoveObject(Vec3(Settings.SnapDistance, 0, 0));
        ScaleObject(Vec3(Settings.SnapDistance, 0, 0));
        break;

      //case Keys::Escape:
      //  ActiveTool = EditorTool::None;
      //  break;



      default:
        //ActiveTool = EditorTool::None;
        break;

      }

    }

    /**************************************************************************/
    /*!
    @brief  Pans the camera when the specified mouse-button is held.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::PanCamera(Vec2 mousePosition)
    {
      if (Settings.Panning) {
        static Vec2 lastPosition;


        auto mousePos = CurrentSpace->getComponent<CameraViewport>()->ScreenToViewport(mousePosition);
        auto camPos = EditorCamera->getComponent<Transform>()->getTranslation();
        // Drag the mouse in the direction opposite the dragging
        

        // If the positions don't match
        if (lastPosition != mousePosition )
          EditorCamera->getComponent<Transform>()->setTranslation(Vec3(mousePos.x - camPos.x, mousePos.y - camPos.y, camPos.z));

        // Save the mouse's last position
        lastPosition = mousePosition;
      }
      

    }

    /**************************************************************************/
    /*!
    @brief  Pans the camera when the specified mouse-button is held.
    @param  event The current mouse position.
    */
    /**************************************************************************/
    void Editor::Hotkeys(Events::KeyDown * event)
    {


    }



  }
}