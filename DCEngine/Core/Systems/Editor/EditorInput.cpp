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

    CommandObjectTransform* TransCommand = NULL;
    /**************************************************************************/
    /*!
    @brief  Receives a MouseDown event.
    @param  event The MouseDown event.
    */
    /**************************************************************************/
    void Editor::OnMouseDownEvent(Events::MouseDown* event)
    {
      if (TransCommand != NULL)
      {
        delete TransCommand;
        TransCommand = NULL;
      }

      if (!Settings.EditorEnabled)
        return;

      if (event->ButtonPressed == MouseButton::Left) {
        // Look for an object that matches the translation
        auto posOnSpace = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
        auto gameObject = FindObjectFromSpace(posOnSpace);
        
        // If the 'Translate' tool is active...
        if (ActiveTool == EditorTool::Translate) {
          // And a valid GameObject was selected, start dragging it
          if (SelectedObject && gameObject && gameObject->getObjectName() != std::string("EditorCamera")) {
            Settings.Dragging = true;
            auto transform = gameObject->getComponent<Components::Transform>();
            TransCommand = new CommandObjectTransform(transform);
            DCTrace << "Editor::OnMouseDownEvent - Dragging: '" << gameObject->getObjectName() << "'\n";
            return;
          }
          //if we have a valid Object selected still
          else if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
            auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
            //and it has a transform
            if (transform != NULL)
            {
              auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
              auto xPos = transform->getTranslation().x;
              auto yPos = transform->getTranslation().y;
              //data for translate editor tools
              Real radius = 8;
              Real arrowTip = 1;
              //if within horizontal tool, drag along x axis
              if (mousePos.x > xPos && mousePos.x < xPos + radius && mousePos.y < yPos + arrowTip && mousePos.y > yPos - arrowTip)
              {
                Settings.DraggingX = true;
                Settings.DragOffset = mousePos.x - xPos;
                
                TransCommand = new CommandObjectTransform(transform);
                DCTrace << "Editor::OnMouseDownEvent - DraggingX: '" << SelectedObject->getObjectName() << "'\n";
                return;
              }
              //if within vertical tool, drag along y axis
              if (mousePos.x > xPos - arrowTip && mousePos.x < xPos + arrowTip && mousePos.y < yPos + radius && mousePos.y > yPos)
              {
                Settings.DraggingY = true;
                Settings.DragOffset = mousePos.y - yPos;
                TransCommand = new CommandObjectTransform(transform);
                DCTrace << "Editor::OnMouseDownEvent - DraggingY: '" << SelectedObject->getObjectName() << "'\n";
                return;
              }
            }
          }
        }
        else if (ActiveTool == EditorTool::Rotate)
        {
          if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
            auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
            if (transform != NULL)
            {
              auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
              auto xPos = transform->getTranslation().x;
              auto yPos = transform->getTranslation().y;
              Real radius = transform->getScale().x *2.5;
              Real radiusSquared = radius*radius;
              Real distanceSquared = (mousePos.x - xPos)*(mousePos.x - xPos) + (mousePos.y - yPos)*(mousePos.y - yPos);
              if (distanceSquared - radiusSquared < 0.25 && distanceSquared - radiusSquared > -0.25)
              {
                Settings.Rotating = true;
                TransCommand = new CommandObjectTransform(transform);
                DCTrace << "Editor::OnMouseDownEvent - Rotating: '" << SelectedObject->getObjectName() << "'\n";
                return;
              }
            }
          }
        }
        else if (ActiveTool == EditorTool::Scale)
        {
          if (SelectedObject && SelectedObject->getObjectName() != std::string("EditorCamera")) {
            auto transform = dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>();
            if (transform != NULL)
            {
              auto mousePos = CurrentSpace->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
              auto xPos = transform->getTranslation().x;
              auto yPos = transform->getTranslation().y;
              Real width = transform->getScale().x *2.5;
              Real height = transform->getScale().y *2.5;
              Real distanceX = mousePos.x - xPos;
              Real distanceY = mousePos.y - yPos;
              if (distanceY - height/2 < 0.25 && distanceY - height/2 > -0.25 && mousePos.x > xPos-width/2 && mousePos.x < xPos + width / 2)
              {
                Settings.ScalingY = true;
                Settings.OriginScale = transform->getScale();
                Settings.OriginMousePos = mousePos;
                TransCommand = new CommandObjectTransform(transform);
                DCTrace << "Editor::OnMouseDownEvent - ScalingY: '" << SelectedObject->getObjectName() << "'\n";
                return;
              }
              else if (distanceX - width / 2 < 0.25 && distanceX - width / 2 > -0.25 && mousePos.y > yPos - height / 2 && mousePos.y < yPos + height / 2)
              {
                Settings.ScalingX = true;
                Settings.OriginScale = transform->getScale();
                Settings.OriginMousePos = mousePos;
                TransCommand = new CommandObjectTransform(transform);
                DCTrace << "Editor::OnMouseDownEvent - ScalingX: '" << SelectedObject->getObjectName() << "'\n";
                return;
              }
            }
          }
        }
        // If an object was found at that position, select it
        if (gameObject)
          SelectObjectFromSpace(gameObject);
        // If no object was found, deselect
        else
          Deselect();

        // If the 'Select' tool is active, attempt to pick it
        //if (ActiveTool == EditorTool::Select) {
          
        //}
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
      if (Settings.Dragging || Settings.DraggingX || Settings.DraggingY) {
        ReleaseObject();
        Settings.Dragging = false;
        Settings.DraggingX = false;
        Settings.DraggingY = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
      }
      if (Settings.Rotating)
      {
        ReleaseObject();
        Settings.Rotating = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
      }
      if (Settings.ScalingX || Settings.ScalingY)
      {
        ReleaseObject();
        Settings.ScalingX = false;
        Settings.ScalingY = false;
        TransCommand->SaveNew(dynamic_cast<GameObject*>(SelectedObject)->getComponent<Components::Transform>());
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        TransCommand = NULL;
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
      RotateObject(event->ScreenPosition);
      ScaleObject(event->ScreenPosition);
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
    @brief  Draws a grid on screen while the editor is enabled.
    */
    /**************************************************************************/
    void Editor::DrawGrid()
    {
      if (!Settings.GridActive)
        return;

      Vec3& cameraPos = EditorCamera->getComponent<Components::Transform>()->getTranslation();
      // The editor grid will always be in front of the camera.
      Vec3 gridStartPos(cameraPos.x, cameraPos.y, cameraPos.z - 1);
      Real edge = 500; // We want to make sure we draw 'very' from far away??
      unsigned lines = 50;
      // Start drawings from the center of the screen and outwards
      for (unsigned int i = 0; i < lines; ++i) {
        // Draw the horizontal lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x -edge, gridStartPos.y + i, gridStartPos.z),
                                                                                 Vec3(gridStartPos.x + edge, gridStartPos.y + i, gridStartPos.z),
                                                                                 Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - edge, gridStartPos.y - i, gridStartPos.z),
                                                                                 Vec3(gridStartPos.x + edge, gridStartPos.y - i, gridStartPos.z),
                                                                                 Settings.GridColor);
        // Draw the vertical lines
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x + i, gridStartPos.y - edge, gridStartPos.z),
                                                                                 Vec3(gridStartPos.x + i, gridStartPos.y + edge, gridStartPos.z),
                                                                                 Settings.GridColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(gridStartPos.x - i, gridStartPos.y - edge, gridStartPos.z),
                                                                                 Vec3(gridStartPos.x - i, gridStartPos.y + edge, gridStartPos.z),
                                                                                 Settings.GridColor);
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