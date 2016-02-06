#pragma once
#include "EditorTool.h"

#include "../../Objects/Entities/EntitiesInclude.h"

namespace DCEngine {
  namespace Systems {

    // Forward declarations    

    class TransformTool : public EditorTool {      
    public:
      void Display();
      void DrawTranslateTool();
      void DrawRotateTool();
      void DrawScaleTool();

      void DragObject(Vec2&);
      void RotateObject(Vec2&);
      void ScaleObject(Vec2&);
      void ReleaseObject();

      void UseTool(GameObjectPtr gameObject, Vec2& position);
      void ReleaseTool();
      bool IsToolRegion(GameObjectPtr);

      enum class Mode {
        Translate,
        Rotate,
        Scale,
      };

      Mode ActiveMode;

    private:

      // Rotate
      bool Rotating = false;
      // Scale
      bool ScalingY = false;
      bool ScalingX = false;

    };

  }
}