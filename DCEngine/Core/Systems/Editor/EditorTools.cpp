/******************************************************************************/
/*!
@file   EditorTools.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Activates the selected tool. Called on update.
    */
    /**************************************************************************/
    void Editor::UseTool()
    {
      switch (ActiveTool) {

      case EditorTool::Select:
        //ShowSelection();
        break;
      case EditorTool::Translate:
        TranslateTool();
        break;
      case EditorTool::Rotate:
        RotateTool();
        break;
      case EditorTool::Scale:
        ScaleTool();
        break;
      default:
        break;

      }
    }

    /**************************************************************************/
    /*!
    @brief  The select tool allows the user to select an object on screen.
    */
    /**************************************************************************/
    void Editor::ShowSelection()
    {      
      if (!SelectedObject)
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        // Do nothing if the GameObject does not have a transform.
        if (!gameObject->HasComponent(std::string("Transform")))
          return;

        // Get the object's position
        auto transform = gameObject->getComponent<Components::Transform>();

        // Draw a selected 'box' around the object
        Vec3 pos = transform->getTranslation();
        Real width = transform->getScale().x * 2;
        Real height = transform->getScale().y * 2;
        Vec4 color(1.0, 0, 0, 1.0);

        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos,
          width, height, color);
      }

    }

    /**************************************************************************/
    /*!
    @brief  The translate tool allows the user to move an object on screen.
    */
    /**************************************************************************/
    void Editor::TranslateTool()
    {
      if (!SelectedObject)
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
        
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        Real radius = 8;
        Real arrowTip = 1;
        Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
        Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
                                         // X-axis
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(radius, 0, 0), xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, -arrowTip, 0), xColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, arrowTip, 0), xColor);
        // Y-axis
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, radius, 0), yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(-arrowTip, -arrowTip, 0), yColor);
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(arrowTip, -arrowTip, 0), yColor);
      }
      
      // Create thin box-colliders on every line

    }

    /**************************************************************************/
    /*!
    @brief  The rotate tool allows the user to rotate an object.
    */
    /**************************************************************************/
    void Editor::RotateTool()
    {
      if (!SelectedObject)
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        Real radius = transform->getScale().x *2.5;
        Vec4 color(0.0f, 0.0f, 1.0f, 1.0);
        if (Settings.Rotating == true)
        {
          auto normal = Vec3(transform->getTranslation().y - Settings.OriginMousePos.y, -(transform->getTranslation().x - Settings.OriginMousePos.x), 0);
          normal *= 10;
          auto negNormal = -normal;
          Vec4 colorR(1.0, 0.0, 0.0, 1.0); // Red
          Vec4 colorG(0.0,1.0, 0.0, 1.0); // Red
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(Vec3(-(transform->getTranslation().y - Settings.OriginMousePos.y), transform->getTranslation().x - Settings.OriginMousePos.x, 0) + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorG);
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawLineSegment(negNormal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), normal + Vec3(Settings.OriginMousePos.x, Settings.OriginMousePos.y, 0), colorR);
        }

        // Draw a selected 'box' around the object
        CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawCircle(pos, radius, color);
      }

    }

    /**************************************************************************/
    /*!
    @brief  The scale tool allows the user to scale an object.
    */
    /**************************************************************************/
    void Editor::ScaleTool()
    {
      if (!SelectedObject)
        return;

        if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
          // Get the object's transform data
          auto transform = gameObject->getComponent<Components::Transform>();
          Vec3 pos = transform->getTranslation();
          Real width = transform->getScale().x *2.5;
          Real height = transform->getScale().y *2.5;
          Vec4 color(0.0f, 0.0f, 1.0f, 1.0);

          // Draw a selected 'box' around the object
          CurrentSpace->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos,
            width, height, color);
        }
      
    }

    /**************************************************************************/
    /*!
    @brief  Moves an object by translation by the specified direction vector.
    @param  direction A vector.
    */
    /**************************************************************************/
    void Editor::MoveObject(Vec3 direction)
    {
      if (!SelectedObject)
        return;

      if (ActiveTool != EditorTool::Translate)
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {
        
        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 pos = transform->getTranslation();
        // Translate the object
        auto TransCommand = new CommandObjectTransform(transform);
        transform->setTranslation(pos + direction);
        TransCommand->SaveNew(transform);
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
        

      }
    }

    /**************************************************************************/
    /*!
    @brief  Scales an object by the specified vector.
    @param  scaleChange The scaling value.
    */
    /**************************************************************************/
    void Editor::ScaleObject(Vec3 scaleChange)
    {
      if (!SelectedObject)
        return;

      if (ActiveTool != EditorTool::Scale)
        return;

      if (auto gameObject = IsSelectableGameObject(SelectedObject)) {

        DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";
        // Get the object's transform data
        auto transform = gameObject->getComponent<Components::Transform>();
        Vec3 scale = transform->getScale();
        // Translate the object
        auto TransCommand = new CommandObjectTransform(transform);
        transform->setScale(scale + scaleChange);
        TransCommand->SaveNew(transform);
        auto command = CommandPtr(TransCommand);
        Settings.Commands.Add(command);
      }

    }


  }
}