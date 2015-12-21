/******************************************************************************/
/*!
\file   DebugReport.h
\author Chen Shu, Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  Manages a viewport, attacheing a camera.
\note   This component is accessed by the graphics system for its
projection matrix.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  enum class DebugDrawType {
    Line,
    Circle,
    Rectangle,
    None,
  };

  namespace Components
  {
    class Transform;
    class DebugReport : public Component {

    public:

      Transform* TransformComponent;
      Boolean ReportTranslation = false;
      DebugDrawType DrawType = DebugDrawType::None;
      Real Radius = 1;
      Real Height = 1;
      Vec3 Offset = Vec3(0, 0, 0);
      Vec4 Color = Vec4(0.5, 0.5, 0.5, 1);
      //Vec3 StartPos = Vec3(0, 0, 0);
      //Vec3 EndPos = Vec3(0, 0, 0);  

      DebugReport(Entity& owner) : Component(std::string("DebugReport"), owner) {}
      void Initialize();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyUp* event);
      void OnMouseDownEvent(Events::MouseDown* event);
      void OnLogicUpdateEvent(Events::LogicUpdate* event);

      void DebugDraw();
    };


  }
}