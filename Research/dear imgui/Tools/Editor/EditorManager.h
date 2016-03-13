/******************************************************************************/
/*
  File:    EditorManager.h
  Author:  Alejandro Hitti
  Date:    11/07/14
  Brief:   Handles the rest of the editor functionality.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

// Includes
//------------------------------------------------------------------------------
#include "GraphicsSystem.h"
#include "Vector3.h"
#include "..\Editor\ImGUI\imgui.h"
#include "Editor_Forward.h"

// Macro for easy access to the singleton
#define EDITOR ((Editor::EditorManager::GetInstance()))

namespace Editor
{
  typedef std::vector<BaseModule*> ModuleVector;
  typedef std::vector<BaseModule*>::iterator ModuleIter;

  namespace Modules
  {
    enum Modules
    {
      Main,
      Scene,
      Profiler,
      Inspector,
      Entity,
      Settings,
      Resources,
      NumModules
    };
  } // Namespace Modules

  namespace RefPoint
  {
    enum RefPoint
    {
      TopLeft,
      TopRight,
      BottomLeft,
      BottomRight,
      Center,
      NumModulePos
    };
  }

  namespace GizmoType
  {
    enum Gizmo
    {
      None,
      Translate,
      Rotate,
      Scale,
      NumGizmos
    };
  }

  struct EditorVertex
  {
    EZ::float3 pos;
    EZ::float4 color;
    EZ::float2 uv;
  };

  struct EditorRects
  {
    RECT rect;
    unsigned vtxOffset;
  };

  struct LayoutFlags
  {
    bool noTitlebar;
    bool noBorder;
    bool noResize;
    bool noMove;
    bool noScrollbar;
    float fillAlpha;
  };

  class EditorManager
  {
  public:
    // Gettor for the singleton instance
    static EditorManager* GetInstance(void)
    {
      static EditorManager instance;
      return &instance;
    }

    // Sets up the editor
    static int Initialize(HWND hWnd, GraphicsSystem* gfx);
    // Updates the state of the editor
    static void Update(void);
    // Handle input through Windows messages
    static LRESULT WINAPI HandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    // Uninitialize the editor
    static void Uninitialize(void);
    // Checks if the editor is active
    static bool IsActive(void);
    // Sets the editor active state to the one passed in
    static void SetActive(bool state);

    // Gets the active state of the selected module
    static bool IsModuleActive(Modules::Modules module);
    // Sets the selected module to be active
    static void SetModuleActive(Modules::Modules module, bool state);
    // Gets the visible state of the selected module
    static bool IsModuleVisible(Modules::Modules module);
    // Sets the selected module to be visible
    static void SetModuleVisible(Modules::Modules module, bool state);
    // Sets the module's position in reference to the point given, + or - the x and y offsets
    static void SetModulePosition(int width, int height, RefPoint::RefPoint refPoint = RefPoint::RefPoint::Center, int xOffset = 0, int yOffset = 0);
    // Get module instance
    static BaseModule* GetModuleInstance(Editor::Modules::Modules module);

    // Sets the state of tooltips. True shows them, false doesn't
    static void SetTooltipState(bool state);
    // Gets the state of tooltips. True is active, false otherwise
    static bool GetTooltipState(void);

    // Sets whether a level was just loaded. Set this to true when loading
    // or refreshing a level.
    static void SetFreshlyLoaded(bool flag);
    // Gets whether a level was just loaded. If the game was unpaused at all
    // since the level was loaded, this will be false
    static bool GetFreshlyLoaded(void);

    // Gets the graphics system
    static GraphicsSystem* GetGFX(void);
    // Gets the Editor Camera
    static GraphicsSystem::ScreenManager::CameraID GetEditorCameraID(void);
    // Get Camera matrix
    static Math::Matrix4& GetEditorCameraMatrix(void);
    // Gets the selected entity
    Core::EntityHandle GetSelectedEntity(void);
    // Sets the selected entity
    void SetSelectedEntity(Core::EntityHandle newHandle);

    // Gets the game's pause state
    static bool IsGamePaused();
    // Sets the game's pause state flag
    static void SetGamePausedFlag(bool state);

    // Gets layout flags and flags
    static LayoutFlags& GetFlags(void);
    static ImGuiWindowFlags& GetLayoutFlags(void);

  private:
    // Default Constructor, Copy Constructor, assignment operator and Destructor
    // declared and not implemented to ensure they can't be called (Singleton)
    EditorManager(void) = default;
    EditorManager(EditorManager&) = delete;
    EditorManager& operator=(const EditorManager&) = delete;
    ~EditorManager(void) = default;

    // ------------------------------------/
    //         ImGUI Base functions
    // ------------------------------------/

    // Initializes ImGUI
    static bool ImGuiInitialize(HWND hWnd);
    // Updates ImGUI
    static void ImGuiUpdate(void);
    // Generates and updates the vertex buffer for drawing
    static void ImGuiDrawList(ImDrawList** const cmdLists, int cmdListsCount);

    // ------------------------------------/
    //         ImGUI Extra functions
    // ------------------------------------/
   
    // Modifies the original style from ImGui into our custom one
    static void ImGuiCustomStyle(void);
    // Layout flags for the editor
    static void InitializeLayoutFlags(void);

    // ------------------------------------/
    //       EditorManager functions
    // ------------------------------------/
    
    // Shows the active modules in the editor
    static void ShowEditor(void);
    // Hides every module in the editor
    static void HideEditor(void);
    // Updates the camera that is active when the editor is also active
    static void UpdateEditorCamera(void);
    // Wraps the mouse within the game window
    static bool WrapMouse(void);

    // Draw the AABB for the selected entity
    static void DrawEntityAABB(Core::Entity* entity);
    // Draws the selected Gizmo
    static void DrawGizmo(void);
    // Updates the position of the gizmo entities
    static void UpdateGizmoPosition(void);
    // Gets input to know what Gizmo to draw
    static void SelectGizmo(void);

    // ------------------------------------/
    //           Static variables
    // ------------------------------------/

    // Vertex and Rect buffers that will draw and clip the editor
    static EditorVertex* m_VertBuffer;
    static EditorRects*  m_RectBuffer;
    static unsigned m_RectCount;
    // Graphics sytem pointer
    static GraphicsSystem* m_Gfx;
    // Container of modules
    static ModuleVector m_Modules;
    // True if editor is active, false otherwise
    static bool m_IsActive;
    // Tooltip bool. True if tooltips are showing, false otherwise
    static bool m_IsTooltipActive;
    // Layout Flags
    static ImGuiWindowFlags m_LayoutFlags;
    // Flags
    static LayoutFlags m_Flags;
    // Bool that stores the pause state of the game
    static bool m_IsGamePaused;

    // Which gizmo is currently in use (Select, Translate, Rotate, Scale)
    static GizmoType::Gizmo m_CurrentGizmoType;
    // Gizmo entities
    static Core::Entity* m_CurrentGizmo[4];
    // Selected entity, if any
    static Core::EntityHandle m_SelectedEntity;

    // Editor's camera to freely move around
    static GraphicsSystem::ScreenManager::CameraID m_EditorCamera;
    // Game's camera to set it back after the editor is closed
    static GraphicsSystem::ScreenManager::CameraID m_GameCamera;
    // Camera's matrix because Zander doesn't want to manage it himself
    static Math::Matrix4 m_CamMatrix;

    // Hacky mouse input
    static Math::Vector3 m_MousePos;
    static Math::Vector3 m_MousePosPrev;

    // Signifies whether a level was just loaded. True if player has a freshly
    // loaded level and false if the game was unpaused since the level was loaded
    static bool m_IsFreshlyLoaded;

  }; // Class EditorManager

} // Namespace Editor
