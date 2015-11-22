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
        SelectTool();
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
    void Editor::SelectTool()
    {
      
      if (!SelectedObject)
        return;

      // Get the object's position
      auto transform = SelectedObject->getComponent<Transform>();      

      // Draw a selected 'box' around the object
      Vec3 pos = transform->getTranslation();
      Real width = transform->getScale().x * 2;
      Real height = transform->getScale().y * 2;
      Vec4 color(1.0, 0, 0, 1.0);
      
      CurrentSpace->getComponent<GraphicsSpace>()->DrawRectangle(pos,
                                                                 width, height, color);
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

      // Get the object's transform data
      auto transform = SelectedObject->getComponent<Transform>();
      Vec3 pos = transform->getTranslation();
      Real radius = 8;
      Real arrowTip = 1;
      Vec4 xColor(1.0, 0.0, 0.0, 1.0); // Red
      Vec4 yColor(0.0, 1.0, 0.0, 1.0); // Green
      // X-axis
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(radius, 0, 0), xColor); 
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, -arrowTip, 0), xColor);
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(radius, 0, 0), pos - Vec3(-arrowTip, arrowTip, 0), xColor);
      // Y-axis
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, radius, 0), yColor);
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(-arrowTip, -arrowTip, 0), yColor);
      CurrentSpace->getComponent<GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, radius, 0), pos - Vec3(arrowTip, -arrowTip, 0), yColor);
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

      auto a = ActiveTool;
      DCTrace << "Editor::MoveObject - Moving '" << SelectedObject->Name() << "' \n";

      // Get the object's transform data
      auto transform = SelectedObject->getComponent<Transform>();
      Vec3 pos = transform->getTranslation();
      // Translate the object
      SelectedObject->getComponent<Transform>()->setTranslation(pos + direction);

    }


  }
}