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

      if (event->ButtonPressed == MouseButton::Left) {
        // Look for an object that matches the translation
        auto posOnSpace = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
        auto gameObject = FindObjectFromSpace(posOnSpace);

        // If an object was found at that position, select it
        if (gameObject)
          SelectObjectFromSpace(gameObject);
        // If no object was found, deselect
        else
          Deselect();

        // If the 'Select' tool is active, attempt to pick it
        //if (ActiveTool == EditorTool::Select) {
          
        //}
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
        Settings.CamStartPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
        Settings.MouseStartPos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position), 0);
      }

		  //if (Settings.Panning == false)
		  //{
			 // //Settings.PositionRecord = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
		  //}
    //    Settings.Panning = true;        
    //  }
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

      case Keys::F8:
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
        // Get the mouse position
        auto mousePos = Vec3(CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(mousePosition), 0);
        auto camPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
        //auto offset = Vec3(mousePos.x - camPos.x, mousePos.y - camPos.y, camPos.z);

        Vec3 cameraDiff = camPos - Settings.CamStartPos;
        Vec3 mouseDiff = mousePos - Settings.MouseStartPos;        

        //float width = 5;
        //CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(mousePos, width, width, Vec4(1, 0, 0, 1));

        // Set the camera's position
        EditorCamera->getComponent<Components::Transform>()->setTranslation(Settings.CamStartPos + cameraDiff - mouseDiff);

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