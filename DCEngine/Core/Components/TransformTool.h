/******************************************************************************/
/*!
@file   TransformTool.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   12/26/2015
@brief  The EditorTransformTool allows the user to translate/rotate/scale
        the currently selected object.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {
  namespace Components
  {
    class Transform;

    class TransformTool : public Component {
    public:

      enum class Mode {
        Translate,
        Rotate,
        Scale,
      };

      #if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(TransformTool, Component);
      #endif
      // Properties
      Real Radius = 5;
      Boolean Snapping = true;
      Real SnapDistance = 0.5;
      Real SnapAngle = 15;     
      DCE_DEFINE_PROPERTY(Real, Radius);
      DCE_DEFINE_PROPERTY(Boolean, Snapping);
      DCE_DEFINE_PROPERTY(Real, SnapDistance);
      DCE_DEFINE_PROPERTY(Real, SnapAngle);
      // Initialization
      TransformTool(Entity& owner) : Component(std::string("TransformTool"), owner) {}
      void Initialize();
      void Select(GameObjectPtr);
      void Deselect();
      // Events
      void OnFrameUpdateEvent(Events::FrameUpdate* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnMouseUpEvent(Events::MouseUp* event);
      void OnMouseUpdateEvent(Events::MouseUpdate* event);
      void OnKeyDownEvent(Events::KeyDown* event);


    private:
      Boolean Active;
      Boolean Dragging;
      Mode Mode;
      GameObjectPtr SelectedObject;
      GameObjectPtr AxisX;
      GameObjectPtr AxisY;
      GameObjectPtr AxisZ;
      Vec4 ColorX = Vec4(1, 0, 0, 1);
      Vec4 ColorY = Vec4(0, 1, 0, 1);
      Vec4 ColorZ = Vec4(0, 0, 1, 1);
      Vec4 ColorHighlight = Vec4(1, 1, 0, 1);

      void Follow();
      void Draw();
      bool CheckBoundaries(Vec2 pos);
      
      void DrawTranslate();
      void DrawScale();
      void DrawRotate();

      bool BoundaryTranslate();
      bool BoundaryRotate();

      void Translate();
      void Scale();
      void Rotate();
      




    };
  }
}