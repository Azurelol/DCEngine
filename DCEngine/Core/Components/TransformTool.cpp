/******************************************************************************/
/*!
@file   TransformTool.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/26/2015
@brief  The EditorTransformTool allows the user to translate/rotate/scale
the currently selected object.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "TransformTool.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(TransformTool, "TransformTool", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(TransformTool);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(TransformTool, Radius);
      DCE_BINDING_DEFINE_PROPERTY(TransformTool, Snapping);
      DCE_BINDING_DEFINE_PROPERTY(TransformTool, SnapDistance);
      DCE_BINDING_DEFINE_PROPERTY(TransformTool, SnapAngle);
    }
    #endif

    /**************************************************************************/
    /*!
    @brief  Initializes the TransformTool.
    */
    /**************************************************************************/
    void TransformTool::Initialize()
    {
      Connect(SpaceRef, Events::FrameUpdate, TransformTool::OnFrameUpdateEvent);
      Connect(Daisy->getMouse(), Events::MouseUpdate, TransformTool::OnMouseUpdateEvent);
      Connect(Daisy->getMouse(), Events::MouseDown,   TransformTool::OnMouseDownEvent);
      Connect(Daisy->getMouse(), Events::MouseUp,     TransformTool::OnMouseUpEvent);
      Connect(Daisy->getKeyboard(), Events::KeyDown,     TransformTool::OnKeyDownEvent);

      SelectedObject = nullptr;
      Active = false;
    }

    /**************************************************************************/
    /*!
    @brief  Sets the TransformTool on the selected object.
    */
    /**************************************************************************/
    void TransformTool::Select(GameObjectPtr object)
    {
      SelectedObject = object;
      Active = true;
    }

    /**************************************************************************/
    /*!
    @brief  Deselects the TransformTool.
    */
    /**************************************************************************/
    void TransformTool::Deselect()
    {
      SelectedObject = nullptr;
      Active = false;
    }

    void TransformTool::OnKeyDownEvent(Events::KeyDown * event)
    {
      switch (event->Key) {

      case Keys::Num2:
        Mode = Mode::Translate;
        DCTrace << "TransformTool::OnKeyDownEvent - Translate \n";
        break;
      case Keys::Num3:
        Mode = Mode::Rotate;
        DCTrace << "TransformTool::OnKeyDownEvent - Rotate \n";
        break;
      case Keys::Num4:
        Mode = Mode::Scale;
        DCTrace << "TransformTool::OnKeyDownEvent - Scale \n";
        break;

      }
    }
    
    void TransformTool::OnMouseDownEvent(Events::MouseDown * event)
    {
      auto mouseToWorldPos = SpaceRef->getComponent<Components::CameraViewport>()->ScreenToViewport(event->Position);
      Dragging = CheckBoundaries(mouseToWorldPos);
    }

    void TransformTool::OnMouseUpEvent(Events::MouseUp * event)
    {
      
      Dragging = false;
    }

    void TransformTool::OnMouseUpdateEvent(Events::MouseUpdate * event)
    {
      
    }


    /**************************************************************************/
    /*!
    @brief  On every logic update...
    */
    /**************************************************************************/
    void TransformTool::OnFrameUpdateEvent(Events::FrameUpdate * event)
    {
      if (!Active)
        return;

      //Follow();
      Draw();

    }

    /**************************************************************************/
    /*!
    @brief  Follows the selected object.
    */
    /**************************************************************************/
    void TransformTool::Follow()
    {
      auto& objectPosition = SelectedObject->getComponent<Components::Transform>()->getTranslation();
      Owner()->getComponent<Components::Transform>()->setTranslation(objectPosition);
    }

    /**************************************************************************/
    /*!
    @brief  Draws the currently active transform tool.
    */
    /**************************************************************************/
    void TransformTool::Draw()
    {
      switch (Mode) {
      case Mode::Translate:
        DrawTranslate();
        break;
      case Mode::Rotate:
        DrawRotate();
        break;
      case Mode::Scale:
        DrawScale();
        break;
      }
    }

    /**************************************************************************/
    /*!
    @brief  Checks for collision along the axes
    */
    /**************************************************************************/
    bool TransformTool::CheckBoundaries(Vec2 pos)
    {
      

      return false;
    }

    /**************************************************************************/
    /*!
    @brief  Draws the Translate tool.
    */
    /**************************************************************************/
    void TransformTool::DrawTranslate()
    {
      auto pos = SelectedObject->getComponent<Components::Transform>()->getTranslation();
      Real arrowTip = 1;
      Real dragRectRadius = Radius / 4;

      // Draw a "dragging" rectangle
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawRectangle(pos, dragRectRadius, dragRectRadius, ColorX);
      // Draws the x-Axis
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(Radius, 0, 0), ColorX);
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(Radius, 0, 0), pos - Vec3(-arrowTip, -arrowTip, 0), ColorX);
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(Radius, 0, 0), pos - Vec3(-arrowTip, arrowTip, 0), ColorX);
      // Draws the y-axis
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, Radius, 0), ColorY);
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, Radius, 0), pos - Vec3(-arrowTip, -arrowTip, 0), ColorY);
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos + Vec3(0, Radius, 0), pos - Vec3(arrowTip, -arrowTip, 0), ColorY);
    }

    /**************************************************************************/
    /*!
    @brief  Draws the Rotate tool.
    */
    /**************************************************************************/
    void TransformTool::DrawRotate()
    {
      // Get the object's transform data
      auto pos = SelectedObject->getComponent<Components::Transform>()->getTranslation();
      // Draw the Z-axis rotation
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawCircle(pos, Radius, ColorZ);
    }


    /**************************************************************************/
    /*!
    @brief  Draws the Scale tool.
    */
    /**************************************************************************/
    void TransformTool::DrawScale()
    {
      auto pos = SelectedObject->getComponent<Components::Transform>()->getTranslation();
      // Draws the x-Axis
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(Radius, 0, 0), ColorX);
      // Draws the y-axis
      SpaceRef->getComponent<Components::GraphicsSpace>()->DrawLineSegment(pos, pos + Vec3(0, Radius, 0), ColorY);
    }


    void TransformTool::Translate()
    {
    }

    void TransformTool::Scale()
    {

    }

    void TransformTool::Rotate()
    {

    }



  }
}