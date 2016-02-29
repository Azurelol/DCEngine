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

      // Update the mouse's starting position
      Settings.MouseStartPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position), 0);

      //==================
      // LEFT MOUSE BUTTON 
      if (event->ButtonPressed == MouseButton::Left) {
        // Look for an object that matches the translation
        auto posOnSpace = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
        auto gameObject = FindObjectFromSpace(posOnSpace);

        // If an object was found at that position...
        if (gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {


          // If the [CTRL] key is being held down, add it (or remove it if already there)
          if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
            SelectionAddOrRemoveToMultiple(gameObject);

          // If we have selected multiple objects already and the object at the position is one of them
          if (IsSelected(gameObject)) {
          }
          // if the object is not currently selected..
          else {
            SelectObjectFromSpace(gameObject);
          }

          // Start using the current tool
          UseTool(gameObject, event->Position);
        }
        // If no object was found, deselect the current object if no tool's region overlaps the selected area
        // and attempt to select multiple objects
        else if (!IsToolRegion(gameObject)) {
          Deselect();
          Selection.Dragging = true;
          Selection.MultiSelectStartPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position), 0);
          SelectMultiple(event->Position);
        }
      }
      //===================
      // RIGHT MOUSE BUTTON
      else if (event->ButtonPressed == MouseButton::Right) {
        Settings.Panning = true;
        Settings.CamStartPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
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
      TransformDrag(event->ScreenPosition);
      CalculateSelectionBounding();
      if (EditorCamera)
        Settings.MouseLastPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->ScreenPosition), 0);
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

      // Stop panning
      if (Settings.Panning)
        Settings.Panning = false;

      // Stop dragging for selection tool
      if (Selection.Dragging) {
        Selection.Dragging = false;
        CalculateSelectionBounding();
      }
      // Stop dragging for transformation tool
      if (Transformation.Dragging) {
        Transformation.Dragging = false;
        TransformDragRelease();
      }
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
        SwitchTool(EditorTools::None);
        break;

      case Keys::Return:
        SwitchTool(EditorTools::None);
        break;

      case Keys::Tilde:
        Windows.ConsoleEnabled = !Windows.ConsoleEnabled;
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
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          DispatchSystemEvents::EditorSave();
        break;

      case Keys::F:
        CenterSelected();
        break;

      case Keys::N:
        if (Daisy->getKeyboard()->KeyIsDown(Keys::LControl))
          Windows.AddResourceEnabled = true;
        break;

      case Keys::F5:
        PlayGame();
        break;

      case Keys::F4:
        Diagnostics.WindowEnabled = !Diagnostics.WindowEnabled;
        break;

      case Keys::Tab:
        Windows.ToolsEnabled = !Windows.ToolsEnabled;
        break;

      case Keys::Num1:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::Select \n";
        SwitchTool(EditorTools::None);
        break;

        // If the editor's transform tool is not a component, we will perform
        // transform operations through the editor's functions.
        if (Settings.TransformTool_IsComponent) {
      case Keys::Num2:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::TranslateTool \n";
        SwitchTool(EditorTools::Translate);
        break;

      case Keys::Num3:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::RotateTool \n";
        SwitchTool(EditorTools::Rotate);
        break;

      case Keys::Num4:
        if (DCE_EDITOR_TRACE_TOOLS)
          DCTrace << "Editor::ScaleTool \n";
        SwitchTool(EditorTools::Scale);
        break;
        }


      case Keys::Delete:
        DeleteObject();
        break;

      case Keys::Up:
        MoveObject(Vec3(0, Settings.SnapDistance, 0));
        break;

      case Keys::Down:
        MoveObject(Vec3(0, -Settings.SnapDistance, 0));
        break;

      case Keys::Left:
        MoveObject(Vec3(-Settings.SnapDistance, 0, 0));
        break;

      case Keys::Right:
        MoveObject(Vec3(Settings.SnapDistance, 0, 0));
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