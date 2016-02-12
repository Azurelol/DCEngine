/******************************************************************************/
/*!
@file   EditorTool.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/6/2016
@brief  The base class from which all editor tools derive from.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

// Engine
#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {
  namespace Systems {

    class Editor;

    /**************************************************************************/
    /*!
    @class EditorTool The base class from which all editor tools derive from.
    */
    /**************************************************************************/
    class EditorTool {
    public:
      //EditorTool();
      EditorTool(Editor& editor);
      virtual void Display() = 0;
      virtual bool WithinRegion(const Vec3& translation, const Vec3& scale) = 0;
      virtual ~EditorTool() {}
      ObjectPtr SelectedObject();
      GameObjectPtr IsSelectable(ObjectPtr);
      SpacePtr CurrentSpace();

    protected:

      // References
      Editor& EditorRef;
      bool Snapping;

    };
    using EditorToolPtr = std::shared_ptr<EditorTool>;

    

    /**************************************************************************/
    /*!
    @class TransformTool Tool for transforming GameObjects.
    */
    /**************************************************************************/
    class TransformTool : public EditorTool {
    public:
      TransformTool(Editor& editor);
      void Display();
      bool WithinRegion(const Vec3& translation, const Vec3& scale);

      void DrawTranslateTool();
      void DrawRotateTool();
      void DrawScaleTool();

      //void TransformDrag(Vec2&);
      //void MoveObject(Vec3);
      //void RotateObject(Vec2&);
      //void ScaleObject(Vec2&);
      //void ScaleObject(Vec3);
      //void ReleaseObject();
      //void UseTool(GameObjectPtr gameObject, Vec2& position);
      //void ReleaseTool();

      enum class Mode {
        Translate,
        Rotate,
        Scale,
      };

      Mode ActiveMode;

    private:

      // Drawing configuration
      float Radius = 5;
      // Boundary Data
      Vec3 XAxisBoundaryEnd;
      Vec3 XAxisMidpoint;
      Vec3 YAxisBoundaryEnd;
      Vec3 YAxisMidpoint;
      Vec3 ZAxisBoundaryEnd;
      Vec3 ZAxisMidpoint;
      // Dragging
      bool Dragging = false;
      bool DraggingX = false;
      bool DraggingY = false;
      float DragOffset = 0;
      // Rotating
      bool Rotating = false;
      // Scaling
      bool ScalingY = false;
      bool ScalingX = false;
      Vec2 OriginMousePos;
      Vec3 OriginScale;

    };

    using TransformToolPtr = std::shared_ptr<TransformTool>;



  }
}