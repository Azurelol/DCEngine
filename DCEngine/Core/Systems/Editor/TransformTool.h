#pragma once
#include "EditorTool.h"

namespace DCEngine {
  namespace Systems {

    // Forward declarations    
    
    class Editor;
    class EditorToolTransform : public EditorTool {
    public:
      EditorToolTransform(DCEngine::Systems::Editor& editor);
      void Display();
      bool WithinRegion(const Vec3&);

      void DrawTranslateTool();
      void DrawRotateTool();
      void DrawScaleTool();

      void DragObject(Vec2&);
      void MoveObject(Vec3);
      void RotateObject(Vec2&);
      void ScaleObject(Vec2&);
      void ScaleObject(Vec3);
      void ReleaseObject();

      void UseTool(GameObjectPtr gameObject, Vec2& position);
      void ReleaseTool();

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
    using TransformToolPtr = std::shared_ptr<EditorToolTransform>;

  }
}