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

#include "../../Engine/Engine.h"

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
      if (!Settings.EditorEnabled)
        return;

      // LEFT MOUSE BUTTON
      if (event->ButtonPressed == MouseButton::Left) {
        // Look for an object that matches the translation
        auto posOnSpace = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
        auto gameObject = FindObjectFromSpace(posOnSpace);        
        if (gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {
          UseTool(gameObject, event->Position);
        }            
        // If an object was found at that position, select it
        if (gameObject)
          SelectObjectFromSpace(gameObject);
        // If no object was found, deselect the current object if no tool's region overlaps the selected area
        // and attempt to select multiple objects
        else if (!IsToolRegion(gameObject)) {
          Deselect();
          Settings.MultiSelectDragging = true;
          Settings.MultiSelectStartPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position), 0);
          SelectMultiple(event->Position);
        }
      }

      // RIGHT MOUSE BUTTON
      else if (event->ButtonPressed == MouseButton::Right) {
        Settings.Panning = true;
        Settings.CamStartPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
        Settings.MouseStartPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position), 0);
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
      if (!Settings.EditorEnabled)
        return;

      ReleaseTool();

      // Stop panning
      if (Settings.Panning)
        Settings.Panning = false;

      // Stop dragging for multiple selection
      if (Settings.MultiSelectDragging) {
        CalculateMultipleSelectedBounding();
        Settings.MultiSelectDragging = false;
      }
    }

    /**************************************************************************/
    /*!
    @brief  Receives a MouseUpdate event.
    @param  event The MouseUpdate event.
    */
    /**************************************************************************/
    void Editor::OnMouseUpdateEvent(Events::MouseUpdate * event)
    {
      SelectMultiple(event->ScreenPosition);
      PanCamera(event->ScreenPosition);
      DragObject(event->ScreenPosition);
      RotateObject(event->ScreenPosition);
      ScaleObject(event->ScreenPosition);

      //if (TransCommand != NULL)
      //{
      //  DragObject(event->ScreenPosition);
      //  RotateObject(event->ScreenPosition, TransCommand->PreviousRotation);
      //  ScaleObject(event->ScreenPosition);
      //}
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

      case Keys::Z:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Undo();
        break;

      case Keys::Y:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Redo();
        break;

      case Keys::X:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Cut();
        break;

      case Keys::C:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Copy();
        break;

      case Keys::V:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Paste();
        break;

      case Keys::D:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Duplicate();
        break;

      case Keys::S:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LShift))
          SelectSpace();
        break;
       
      case Keys::F:
        CenterSelected();
        break;

      case Keys::N:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          WindowAddResourceEnabled = true;
        break;

      case Keys::F5:        
        PlayGame();
        break;

      case Keys::F4:
        WindowDiagnosticsEnabled = !WindowDiagnosticsEnabled;
        break;

      case Keys::Tab:
        WindowToolsEnabled = !WindowToolsEnabled;
        break;

      case Keys::Num1:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::SelectTool \n";
        ActiveTool = EditorTool::Select;
        break;

      // If the editor's transform tool is not a component, we will perform
      // transform operations through the editor's functions.
      if (Settings.TransformTool_IsComponent) {
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
        }


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

      default:
        break;
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