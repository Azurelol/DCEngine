#pragma once

#include "../../Engine/Data.h"
#include "../../Engine/Command.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @struct Configuration data for the Editor system.
  */
  /**************************************************************************/
  struct EditorConfig {
    // Project
    bool EditorEnabled = false;
    std::string ProjectsPath;
    std::string RecentProject;
    ProjectData* ProjectInfo;
    // Tools
    bool TransformTool_IsComponent = false;
    bool GridActive = true;
    Real GridLength = 1.0f;
    Vec4 GridColor = Vec4(0.5f, 0.5f, 0.5f, 0.1f);
    // Multi-selection
    bool MultiSelectDragging = false;
    Vec4 MultiSelectColor = Vec4(0.3, 0.7, 0.3f, 0.5f);
    // Snapping
    bool Snapping = true;
    float SnapDistance = 1.0;
    float SnapAngle = 15;
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
    //Panning
    bool Panning = false;
    Vec3 CamStartPos;
    Vec3 MouseStartPos;
    // Commands
    CommandManager Commands;
  };

  struct SelectionData {
    Vec3 MultiSelectMidpoint;
    Vec3 MultiSelectStartPos;
    Vec3 MultiSelectArea;

    Vec3  SelectedBoundingCenter;
    float SelectedBoundingWidth;
    float SelectedBoundingHeight;


  };

  namespace Systems {




  }
}