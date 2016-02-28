#pragma once

#include "../../Engine/Data.h"
#include "../../Engine/Command.h"
#include "../../Engine/Timer.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @struct Editor Configuration data for the Editor system.
  */
  /**************************************************************************/
  struct EditorConfig {
    // Project
    Time::Timer AutoSaveTimer;
    bool EditorEnabled = false;
    bool LightningEnabled = false;
    std::string ProjectsPath;
    std::string RecentProject;
    ProjectProperties* ProjectProperties;
    // Window
    Vec2 ViewportResize;
    // Tools
    bool TransformTool_IsComponent = false;
    bool GridActive = true;
    Real GridLength = 1.0f;
    Vec4 GridColor = Vec4(0.5f, 0.5f, 0.5f, 0.1f);
    // Snapping
    bool Snapping = true;
    float SnapDistance = 1.0f;
    float SnapAngle = 15.0f;
    //Panning
    bool Panning = false;
    Vec3 CamStartPos;
    Vec3 CameraLastPos;
    Vec3 MouseStartPos;
    Vec3 MouseLastPos;
    // Commands
    CommandManager Commands;
    // Console
    size_t ConsoleWindowStringLength = 0;
    size_t PreviousConsoleWindowStringLength = 0;
    EditorConfig() : AutoSaveTimer(60.0f, Time::Timer::Mode::Countdown, true),  
                     CameraLastPos(0.0f, 0.0f, 40.0f) {}
  };

  /**************************************************************************/
  /*!
  @struct SelectionData Data about the currently selected object(s).
  */
  /**************************************************************************/
  struct SelectionData {
    // Multi-selection
    bool Dragging = false;
    Vec4 MultiSelectColor = Vec4(0.3, 0.7, 0.3f, 0.5f);

    Vec3 MultiSelectMidpoint;
    Vec3 MultiSelectStartPos;
    Vec3 MultiSelectArea;

    GameObjectRawVec SelectedAtZ;
    Vec3  SelectedBoundingCenter;
    float SelectedBoundingWidth;
    float SelectedBoundingHeight;
  };

  /**************************************************************************/
  /*!
  @struct TransformToolData Data about the current transform tool.
  */
  /**************************************************************************/
  struct TransformToolData {
    // Dragging
    bool Dragging = false;
    bool DraggingX = false;
    bool DraggingY = false;
    float DragOffset = 0;
	Vec3 LastMousePos;

    TransformDataPairVec InitialGameObjectTransforms;
    // Rotating
    bool Rotating = false;
    // Scaling
    bool ScalingY = false;
    bool ScalingX = false;
    Vec2 OriginMousePos;
    Vec3 OriginScale;
    // Boundaries
    float Radius = 5;
    Vec3 XAxisBoundaryEnd;
    Vec3 XAxisMidpoint;
    Vec3 YAxisBoundaryEnd;
    Vec3 YAxisMidpoint;
    Vec3 ZAxisBoundaryEnd;
    Vec3 ZAxisMidpoint;
  };

  /**************************************************************************/
  /*!
  @struct EditorWindows Flags to enable different editor windows.
  */
  /**************************************************************************/
  struct EditorWindows {
    bool SplashScreenEnabled = true;
    bool MenuBarEnabled = false;
    bool LevelEnabled = false;
    bool AddResourceEnabled = false;
    bool ObjectsEnabled = false;
    bool PropertiesEnabled = false;
    bool LibraryEnabled = false;
    bool DiagnosticsEnabled = false;
    bool SaveLevelEnabled = false;
    bool LoadLevelEnabled = false;
    bool ConsoleEnabled = false;
    bool ToolsEnabled = false;
    bool CreateFromArchetypeEnabled = false;
    bool CollisionTableEditorEnabled = false;
    bool SpriteLayerOrderEditorEnabled = false;
    bool CommandsEnabled = false;
  };

  namespace Systems {




  }
}