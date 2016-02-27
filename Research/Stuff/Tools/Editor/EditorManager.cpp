/******************************************************************************/
/*
  File:    EditorManager.cpp
  Author:  Alejandro Hitti
  Date:    11/07/14
  Brief:   Handles the rest of the editor functionality.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
  */
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"
#include "EditorManager.h"

#include "GraphicsSystem.h"
#include "Input.h"

// Modules
#include "..\Editor\Modules\BaseModule.h"
#include "..\Editor\Modules\MainModule.h"
#include "..\Editor\Modules\SceneModule.h"
#include "..\Editor\Modules\ProfilerModule.h"
#include "..\Editor\Modules\InspectorModule.h"
#include "..\Editor\Modules\EntityModule.h"
#include "..\Editor\Modules\SettingsModule.h"
#include "..\Editor\Modules\ResourcesModule.h"

const unsigned NumberOfVerts = 30000;
const unsigned NumberOfRecs = 1000;
const Math::floating CamMoveSpeed = 5.0f;
const Math::floating CamRotSpeed = 0.2f;
const Math::floating MouseDeadzone = 0.5f;
const float GizmoSize = 20.0f;

namespace Editor
{
  EditorVertex* EditorManager::m_VertBuffer = new EditorVertex[NumberOfVerts];
  EditorRects*  EditorManager::m_RectBuffer = new EditorRects[NumberOfRecs];
  GraphicsSystem* EditorManager::m_Gfx = nullptr;
  ModuleVector EditorManager::m_Modules;
  bool EditorManager::m_IsActive = false;
  bool EditorManager::m_IsTooltipActive = false;
  bool EditorManager::m_IsFreshlyLoaded = false;
  ImGuiWindowFlags EditorManager::m_LayoutFlags;
  LayoutFlags EditorManager::m_Flags;
  bool EditorManager::m_IsGamePaused = EDITOR->IsGamePaused();
  unsigned EditorManager::m_RectCount = 0;
  // Editor's camera to freely move around
  GraphicsSystem::ScreenManager::CameraID EditorManager::m_EditorCamera;
  // Game's camera to set it back after the editor is closed
  GraphicsSystem::ScreenManager::CameraID EditorManager::m_GameCamera;
  Math::Matrix4 EditorManager::m_CamMatrix = Math::Matrix4::Identity;
  // Hacky mouse input
  Math::Vector3 EditorManager::m_MousePos;
  Math::Vector3 EditorManager::m_MousePosPrev;
  Core::EntityHandle EditorManager::m_SelectedEntity = Core::c_InvalidHandle;
  GizmoType::Gizmo EditorManager::m_CurrentGizmoType = GizmoType::None;
  Core::Entity* EditorManager::m_CurrentGizmo[4] = { nullptr };

  INT64 ticksPerSecond = 0;
  INT64 time = 0;

  // Sets up the editor
  int EditorManager::Initialize(HWND hWnd, GraphicsSystem* gfx)
  {
    m_Gfx = gfx;
    ImGuiInitialize(hWnd);
    ImGuiCustomStyle();
    InitializeLayoutFlags();
    m_EditorCamera = m_Gfx->GetScreenManager().AddCamera();

    // Add the modules to the ModulesVector
    m_Modules.resize(Modules::NumModules);

    m_Modules[Modules::Main] = MainModule::GetInstance();
    m_Modules[Modules::Scene] = SceneModule::GetInstance();
    m_Modules[Modules::Profiler] = ProfilerModule::GetInstance();
    m_Modules[Modules::Inspector] = InspectorModule::GetInstance();
    m_Modules[Modules::Entity] = EntityModule::GetInstance();
    m_Modules[Modules::Settings] = SettingsModule::GetInstance();
    m_Modules[Modules::Resources] = ResourcesModule::GetInstance();

    for (int i = 0; i < Modules::NumModules; ++i)
      m_Modules[i]->Initialize();

    for (unsigned i = 0; i < 4; ++i)
    {
      m_CurrentGizmo[i] = g_Memory.Allocate<Core::Entity>();
      m_CurrentGizmo[i] = g_Factory.CreateEntityUnmanaged("Gizmo");
      m_CurrentGizmo[i]->Initialize();
    }

    return true;
  }

  // Updates the state of the editor
  void EditorManager::Update(void)
  {
    if (Input::Keyboard::IsKeyTriggered(Input::Keyboard::Tilde))
    {
      m_IsActive = !m_IsActive;
      m_Gfx->DrawEditor(m_IsActive);

      if (m_IsActive)
      {
        g_Messaging.BroadcastMessage(Core::MessageName::PauseGame, 0, 0);
        m_GameCamera = m_Gfx->GetScreenManager().GetActiveCameraID();
        m_Gfx->GetScreenManager().SetActiveCamera(m_EditorCamera);
      }
      else
      {
        g_Messaging.BroadcastMessage(Core::MessageName::UnpauseGame, 0, 0);
        m_Gfx->GetScreenManager().SetActiveCamera(m_GameCamera);
      }
    }

    // If game gets unpaused after a level is loaded then it is no longer
    // considered "Freshly Loaded"
    if (m_IsFreshlyLoaded == true)
    {
      if (g_Core.IsPaused() == false)
        m_IsFreshlyLoaded = false;
    }

    // Returns early if editor is not active
    if (!m_IsActive)
      return;

    // Editor is active
    else
    {
      ImGuiUpdate();

      UpdateEditorCamera();
      m_MousePosPrev = m_MousePos;

      // Shows tooltips when SHIFT is held
      if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Shift))
        SetTooltipState(true);
      if (Input::Keyboard::IsKeyReleased(Input::Keyboard::Shift))
        SetTooltipState(false);

        //TEST CODE GET AIDS
        //You can delete this, just leaving it as an example
      /*
      ImGuiIO& io = ImGui::GetIO();

      if (io.MouseDown[0])
      {
          g_Picking.Pick(m_CamMatrix, (long)m_MousePos.X, (long)m_MousePos.Y);
      }
      */
        //TEST CODE GET AIDS
        //You can delete this, just leaving it as an example

      static bool isGizmoGrabbed[4] = { false, false, false, false };

      for (unsigned i = 0; i < 4; ++i)
        m_CurrentGizmo[i]->Update(1.0f);

      bool isAGizmoGrabbed = isGizmoGrabbed[0] || isGizmoGrabbed[1] || isGizmoGrabbed[2] || isGizmoGrabbed[3];

      if (ImGui::GetIO().MouseDown[0] && !ImGui::IsMouseHoveringAnyWindow())
      {
        for (unsigned i = 0; i < 4; ++i)
        {
          if ((g_Picking.Cast(m_CamMatrix, (long)m_MousePos.X, (long)m_MousePos.Y, m_CurrentGizmo[i]->GetComponentAs<Core::TransformComponent>()) && !isAGizmoGrabbed) || isGizmoGrabbed[i])
          {
            isGizmoGrabbed[i] = true;
            DrawEntityAABB(m_CurrentGizmo[i]);
            break;
          }
        }

        if (!isGizmoGrabbed[0] && !isGizmoGrabbed[1] && !isGizmoGrabbed[2] && !isGizmoGrabbed[3])
          m_SelectedEntity = (g_Picking.Pick(m_CamMatrix, (long)m_MousePos.X, (long)m_MousePos.Y))->GetHandle();
      }

      if (!ImGui::GetIO().MouseDown[0])
        for (unsigned i = 0; i < 4; ++i)
          isGizmoGrabbed[i] = false;

      // Update all modules
      for (ModuleIter module = m_Modules.begin(); module != m_Modules.end(); ++module)
        (*module)->Update();

      SelectGizmo();

      // THIS IS TEMPORARY
      // Hides Inspector module if the mouse is clicked outside of any editor window
      //if (ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
      //  m_SelectedEntity = Core::c_InvalidHandle;

      // Functions that get called if there is an entity selected
      if (m_SelectedEntity != Core::c_InvalidHandle && g_Factory.GetEntity(m_SelectedEntity))
      {
        // Show selected entity's AABB if one is selected
        DrawEntityAABB(g_Factory.GetEntity(m_SelectedEntity));
        // Update gizmo positions
        UpdateGizmoPosition();
        // Show selected entity's Gizmo
        DrawGizmo();
      }

      ImGui::Render();
    }
  }

  // Handle input through Windows messages
  LRESULT EditorManager::HandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
  {
    ImGuiIO& io = ImGui::GetIO();

    switch (msg)
    {
      case WM_LBUTTONDOWN:
        io.MouseDown[0] = true;
        return true;
      case WM_LBUTTONUP:
        io.MouseDown[0] = false;
        return true;
      case WM_RBUTTONDOWN:
        io.MouseDown[1] = true;
        return true;
      case WM_RBUTTONUP:
        io.MouseDown[1] = false;
        return true;
      case WM_MOUSEWHEEL:
        io.MouseWheel = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1;
        return true;
      case WM_MOUSEMOVE:
        m_MousePos.X = (signed short)(lParam);
        m_MousePos.Y = (signed short)(lParam >> 16);

        io.MousePos.x = (signed short)(lParam);
        io.MousePos.y = (signed short)(lParam >> 16);
        return true;
      case WM_CHAR:
        if (wParam > 0 && wParam < 0x10000 && !(ImGui::GetIO().MouseDown[1]))
          io.AddInputCharacter((unsigned short)wParam);
        return true;
    }

    return false;
  }

  // Uninitialize the editor
  void EditorManager::Uninitialize(void)
  {
    delete[] m_VertBuffer;
    delete[] m_RectBuffer;

    for (unsigned i = 0; i < 4; ++i)
    {
      m_CurrentGizmo[i]->Uninitialize();
      g_Memory.Free(m_CurrentGizmo[i]);
      m_CurrentGizmo[i] = nullptr;
    }

    // Uninitialize all modules
    for (ModuleIter module = m_Modules.begin(); module != m_Modules.end(); ++module)
    {
      if (*module)
        (*module)->~BaseModule();
    }

    ImGui::Shutdown();
  }

  // Checks if the editor is active
  bool EditorManager::IsActive(void)
  {
    return m_IsActive;
  }

  // Sets the editor active state to the one passed in
  void EditorManager::SetActive(bool state)
  {
    m_IsActive = state;
  }

  // Gets the active state of the selected module
  bool EditorManager::IsModuleActive(Modules::Modules module)
  {
    return m_Modules[module]->IsActive();
  }

  // Sets the selected module to be active
  void EditorManager::SetModuleActive(Modules::Modules module, bool state)
  {
    m_Modules[module]->SetActive(state);
  }

  // Gets the visible state of the selected module
  bool EditorManager::IsModuleVisible(Modules::Modules module)
  {
    return m_Modules[module]->IsVisible();
  }

  // Sets the selected module to be visible
  void EditorManager::SetModuleVisible(Modules::Modules module, bool state)
  {
    m_Modules[module]->SetVisible(state);
  }

  // Get module instance
  BaseModule* EditorManager::GetModuleInstance(Editor::Modules::Modules module)
  {
    return m_Modules[module];
  }

  // Gets the graphics system
  GraphicsSystem* EditorManager::GetGFX(void)
  {
    return m_Gfx;
  }

  // Gets the Editor Camera
  GraphicsSystem::ScreenManager::CameraID EditorManager::GetEditorCameraID(void)
  {
    return m_EditorCamera;
  }

  // Get Camera matrix
  Math::Matrix4& EditorManager::GetEditorCameraMatrix(void)
  {
    return m_CamMatrix;
  }

  // Gets the selected entity
  Core::EntityHandle EditorManager::GetSelectedEntity(void)
  {
    return m_SelectedEntity;
  }

  // Sets the selected entity
  void EditorManager::SetSelectedEntity(Core::EntityHandle newHandle)
  {
    m_SelectedEntity = newHandle;
  }

  // Gets the game's pause state
  bool EditorManager::IsGamePaused()
  {
    return m_IsGamePaused;
  }

  // Sets the game's pause state flag
  void EditorManager::SetGamePausedFlag(bool state)
  {
    m_IsGamePaused = state;
  }

  // Gets layout flags and flags
  LayoutFlags& EditorManager::GetFlags(void)
  {
    return m_Flags;
  }

  ImGuiWindowFlags& EditorManager::GetLayoutFlags(void)
  {
    return m_LayoutFlags;
  }


  // ------------------------------------/
  //         ImGUI Base functions
  // ------------------------------------/

  // Initializes ImGUI
  bool EditorManager::ImGuiInitialize(HWND hWnd)
  {
    RECT rect;
    GetClientRect(hWnd, &rect);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
    io.DeltaTime = 1.0f / 60.0f;
    io.PixelCenterOffset = 0.0f;

    io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_UP;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    io.KeyMap[ImGuiKey_A] = 'A';
    io.KeyMap[ImGuiKey_C] = 'C';
    io.KeyMap[ImGuiKey_V] = 'V';
    io.KeyMap[ImGuiKey_X] = 'X';
    io.KeyMap[ImGuiKey_Y] = 'Y';
    io.KeyMap[ImGuiKey_Z] = 'Z';

    io.RenderDrawListsFn = ImGuiDrawList;

    // Load font texture
    const void* pngData;
    unsigned int pngSize;

    ImGui::GetDefaultFontData(nullptr, nullptr, &pngData, &pngSize);

    m_Gfx->GetImguiAPI().CreateBuffer(static_cast<void*>(m_VertBuffer), sizeof(EditorVertex)* NumberOfVerts);
    m_Gfx->LoadTextureFromMemory("ImGuiTexture", pngData, pngSize);
    m_Gfx->GetImguiAPI().SetTexture(m_Gfx->GetTexture("ImGuiTexture"));

    return true;
  }

  // Updates ImGUI
  void EditorManager::ImGuiUpdate(void)
  {
    ImGuiIO& io = ImGui::GetIO();

    // Setup timestep
    INT64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    io.DeltaTime = (float)(currentTime - time) / ticksPerSecond;
    time = currentTime;

    // Setup inputs
    BYTE keyState[256];
    GetKeyboardState(keyState);

    for (int i = 0; i < 256; ++i)
      io.KeysDown[i] = (keyState[i] & 0x80) != 0;

    io.KeyCtrl = (keyState[VK_CONTROL] & 0x80) != 0;
    io.KeyShift = (keyState[VK_SHIFT] & 0x80) != 0;

    // Start the frame
    ImGui::NewFrame();
  }

  void EditorManager::ImGuiDrawList(ImDrawList** const cmdLists, int cmdListsCount)
  {
    // Get the total vertex count
    size_t totalVtxCount = 0;
    EditorVertex* vtxBufferIter = m_VertBuffer;

    for (int i = 0; i < cmdListsCount; ++i)
      totalVtxCount += cmdLists[i]->vtx_buffer.size();

    if (totalVtxCount == 0)
      return;

    // Copy and concert all vertices into a single contiguous buffer
    for (int i = 0; i < cmdListsCount; ++i)
    {
      const ImDrawList* cmdList = cmdLists[i];
      const ImDrawVert* vtxSrc = &cmdList->vtx_buffer[0];

      for (size_t j = 0; j < cmdList->vtx_buffer.size(); ++j)
      {
        vtxBufferIter->pos.x = vtxSrc->pos.x - ImGui::GetIO().DisplaySize.x / 2.0f;
        vtxBufferIter->pos.y = ImGui::GetIO().DisplaySize.y / 2.0f - vtxSrc->pos.y;
        vtxBufferIter->pos.z = 0.0f;

        vtxBufferIter->color.x = (float)(vtxSrc->col & 0x000000FF) / 255.0f;
        vtxBufferIter->color.y = (float)((vtxSrc->col & 0x0000FF00) >> 8) / 255.0f;
        vtxBufferIter->color.z = (float)((vtxSrc->col & 0x00FF0000) >> 16) / 255.0f;
        vtxBufferIter->color.w = (float)((vtxSrc->col & 0xFF000000) >> 24) / 255.0f;

        vtxBufferIter->uv.x = vtxSrc->uv.x;
        vtxBufferIter->uv.y = vtxSrc->uv.y;

        ++vtxBufferIter;
        ++vtxSrc;
      }
    }

    // Clip Rects
    int rectCount = 0;
    int numCommands = 0;
    for (int i = 0; i < cmdListsCount; ++i)
    {
      // Render command list
      const ImDrawList* cmdList = cmdLists[i];

      for (const ImDrawCmd* cmdIter = cmdList->commands.begin(); cmdIter != cmdList->commands.end(); ++cmdIter, ++numCommands)
      {
        m_RectBuffer[numCommands].rect = { (LONG)cmdIter->clip_rect.x, (LONG)cmdIter->clip_rect.y, (LONG)cmdIter->clip_rect.z, (LONG)cmdIter->clip_rect.w };
        m_RectBuffer[numCommands].vtxOffset = cmdIter->vtx_count;
        rectCount++;
      }
    }

    m_Gfx->GetImguiAPI().SetRectData((ImguiAPI::ClipRect*)m_RectBuffer);

    m_Gfx->GetImguiAPI().Update(totalVtxCount, rectCount);
  }

  // ------------------------------------/
  //         ImGUI Extra functions
  // ------------------------------------/

  // Sets the state of tooltips. True shows them, false doesn't
  void EditorManager::SetTooltipState(bool state)
  {
    m_IsTooltipActive = state;
  }

  // Gets the state of tooltips. True is active, false otherwise
  bool EditorManager::GetTooltipState(void)
  {
    return m_IsTooltipActive;
  }

  // Sets whether level was freshly loaded. True means a level was loaded
  void EditorManager::SetFreshlyLoaded(bool flag)
  {
    m_IsFreshlyLoaded = flag;
  }

  // True if level freshly loaded. False if the game was unpaused since
  // the level was loaded.
  bool EditorManager::GetFreshlyLoaded(void)
  {
    return m_IsFreshlyLoaded;
  }

  // Modifies the original style from ImGui into our custom one
  void EditorManager::ImGuiCustomStyle(void)
  {
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 5;
  }

  // Layout flags for the editor
  void EditorManager::InitializeLayoutFlags(void)
  {
    m_Flags.noTitlebar = false;
    m_Flags.noBorder = true;
    m_Flags.noResize = false;
    m_Flags.noMove = false;
    m_Flags.noScrollbar = false;
    m_Flags.fillAlpha = 0.85f;

    m_LayoutFlags = (m_Flags.noTitlebar ? ImGuiWindowFlags_NoTitleBar : 0) |
                    (m_Flags.noBorder ? 0 : ImGuiWindowFlags_ShowBorders)  |
                    (m_Flags.noResize ? ImGuiWindowFlags_NoResize : 0)     |
                    (m_Flags.noMove ? ImGuiWindowFlags_NoMove : 0)         |
                    (m_Flags.noScrollbar ? ImGuiWindowFlags_NoScrollbar : 0);
  }

  // ------------------------------------/
  //       EditorManager functions
  // ------------------------------------/

  // Shows the active modules in the editor
  void EditorManager::ShowEditor(void)
  {
    for (int i = 0; i < Modules::NumModules; ++i)
    if (m_Modules[i]->IsActive())
      m_Modules[i]->SetVisible(true);
  }

  // Hides every module in the editor
  void EditorManager::HideEditor(void)
  {
    for (int i = 0; i < Modules::NumModules; ++i)
      m_Modules[i]->SetVisible(false);
  }

  // Sets the module's position in reference to the point given, + or - the x and y offsets
  void EditorManager::SetModulePosition(int width, int height, RefPoint::RefPoint refPoint, int xOffset, int yOffset)
  {
    // Get the window resolution
    int wndWidth = m_Gfx->GetResolution().width;
    int wndHeight = m_Gfx->GetResolution().height;
    int xPos;
    int yPos;

    // Which reference point
    switch (refPoint)
    {
      case Editor::RefPoint::TopLeft:
        xPos = 0 + xOffset;
        yPos = 0 + yOffset;
        break;

      case Editor::RefPoint::TopRight:
        xPos = wndWidth - width + xOffset;
        yPos = 0 + yOffset;
        break;

      case Editor::RefPoint::BottomLeft:
        xPos = 0 + xOffset;
        yPos = wndHeight - height + yOffset;
        break;

      case Editor::RefPoint::BottomRight:
        xPos = wndWidth - width + xOffset;
        yPos = wndHeight - height + yOffset;
        break;

      case Editor::RefPoint::Center:
        xPos = (wndWidth - width) / 2 + xOffset;
        yPos = (wndHeight - height) / 2 + yOffset;
        break;

      default:
        break;
    }

    // Solves edge case when the module is bigger than the window
    if (xPos < 0)
      xPos = 0;
    if (yPos < 0)
      yPos = 0;

    ImGui::SetWindowPos(ImVec2((float)xPos, (float)yPos));
  }

  void EditorManager::UpdateEditorCamera()
  {
    // Camera can only be moved when the right mouse click is pressed
    if (!ImGui::GetIO().MouseDown[1])
      return;

    EZ::Camera& camera = GetGFX()->GetScreenManager().GetActiveCamera();
    EZ::float3 camPos = camera.GetPosition();
    static EZ::float3 camLook = EZ::float3(0, 0, 1);

    static Math::floating mouseDeltaX = m_MousePos.X - m_MousePosPrev.X;
    static Math::floating mouseDeltaY = m_MousePos.Y - m_MousePosPrev.Y;

    std::string debug = "";
    debug += "DeltaX: ";
    debug += std::to_string(mouseDeltaX);
    debug += " ";
    debug += std::to_string(mouseDeltaY);
    debug += '\n';

    OutputDebugString(debug.c_str());

    if (!WrapMouse())
    {
      mouseDeltaX = m_MousePos.X - m_MousePosPrev.X;
      mouseDeltaY = m_MousePos.Y - m_MousePosPrev.Y;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::W))
    {
      Math::Vector3 zAxis = m_CamMatrix.GetColumn(2);
      zAxis.Normalize();

      *(Math::Vector3*)&camPos += CamMoveSpeed * zAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::S))
    {
      Math::Vector3 zAxis = m_CamMatrix.GetColumn(2);
      zAxis.Normalize();

      *(Math::Vector3*)&camPos -= CamMoveSpeed * zAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::A))
    {
      Math::Vector3 xAxis = m_CamMatrix.GetColumn(0);
      xAxis.Normalize();

      *(Math::Vector3*)&camPos += CamMoveSpeed * xAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::D))
    {
      Math::Vector3 xAxis = m_CamMatrix.GetColumn(0);
      xAxis.Normalize();

      *(Math::Vector3*)&camPos -= CamMoveSpeed * xAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::E) || Input::Keyboard::IsKeyHeld(Input::Keyboard::Space))
    {
      Math::Vector3 yAxis = m_CamMatrix.GetColumn(1);
      yAxis.Normalize();

      *(Math::Vector3*)&camPos += CamMoveSpeed * yAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Q) || Input::Keyboard::IsKeyHeld(Input::Keyboard::LeftShift))
    {
      Math::Vector3 yAxis = m_CamMatrix.GetColumn(1);
      yAxis.Normalize();

      *(Math::Vector3*)&camPos -= CamMoveSpeed * yAxis;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Left) || mouseDeltaX < -MouseDeadzone)
    {
      Math::floating degrees = CamRotSpeed * mouseDeltaX;

      Math::Quaternion quat(Math::Vector3::UnitY, degrees);
      Math::Matrix4 mat(quat);

      *((Math::Vector3*)&camLook) *= mat;

      //Update cam matrix
      m_CamMatrix = mat * m_CamMatrix;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Right) || mouseDeltaX > MouseDeadzone)
    {
      Math::floating degrees = CamRotSpeed * mouseDeltaX;

      Math::Quaternion quat(Math::Vector3::UnitY, degrees);
      Math::Matrix4 mat(quat);

      *((Math::Vector3*)&camLook) *= mat;

      //Update cam matrix
      m_CamMatrix = mat * m_CamMatrix;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Up) || mouseDeltaY < -MouseDeadzone)
    {
      Math::floating degrees = -CamRotSpeed * mouseDeltaY;

      Math::Vector3 vec(Math::Quaternion(m_CamMatrix).GetEuler());

      if ((vec.X - degrees) <= -88)
        degrees = 0;

      Math::Quaternion quat(m_CamMatrix.GetColumn(0), degrees);
      quat.Normalize();
      Math::Matrix4 mat(quat);

      *((Math::Vector3*)&camLook) *= mat;
      camLook.Normalize();

      //Update cam matrix
      m_CamMatrix = mat * m_CamMatrix;
    }

    if (Input::Keyboard::IsKeyHeld(Input::Keyboard::Down) || mouseDeltaY > MouseDeadzone)
    {
      Math::floating degrees = -CamRotSpeed * mouseDeltaY;

      Math::Vector3 vec(Math::Quaternion(m_CamMatrix).GetEuler());

      if ((vec.X - degrees) >= 88)
        degrees = 0;

      Math::Quaternion quat(m_CamMatrix.GetColumn(0), degrees);
      quat.Normalize();
      Math::Matrix4 mat(quat);

      *((Math::Vector3*)&camLook) *= mat;
      camLook.Normalize();

      //Update cam matrix
      m_CamMatrix = mat * m_CamMatrix;
    }

    camLook.Normalize();

    camera.LookAt(camLook);
    camera.SetPosition(camPos);
  }

  bool EditorManager::WrapMouse(void)
  {
    POINT mousePos;
    RECT clientRect;
    RECT windowRect;
    bool isChanged = false;

    GetClientRect(GetGFX()->GetHWND(), &clientRect);
    GetWindowRect(GetGFX()->GetHWND(), &windowRect);
    GetCursorPos(&mousePos);

    //std::string debug = "mousePos:";
    //debug += std::to_string(mousePos.x);
    //debug += " - ";
    //debug += std::to_string(mousePos.y);

    //debug += '\n';

    //debug += "m_MousePos: ";
    //debug += std::to_string(m_MousePos.X);
    //debug += " - ";
    //debug += std::to_string(m_MousePos.Y);
    //debug += "\n\n";

    //OutputDebugString(debug.c_str());

    if (m_MousePos.X > clientRect.right)
    {
      mousePos.x = windowRect.left;
      isChanged = true;
    }

    if (m_MousePos.X < clientRect.left)
    {
      mousePos.x = windowRect.right;
      isChanged = true;
    }

    if (m_MousePos.Y < clientRect.top)
    {
      mousePos.y = windowRect.bottom;
      isChanged = true;
    }

    if (m_MousePos.Y > clientRect.bottom)
    {
      mousePos.y = windowRect.top;
      isChanged = true;
    }

    if (isChanged)
    {
      SetCursorPos((int)mousePos.x, (int)mousePos.y);
      isChanged = true;
    }

    return isChanged;
  }

  // Draw the AABB for the selected entity
  void EditorManager::DrawEntityAABB(Core::Entity* entity)
  {
    const Graphics3DComponent* currGraphicsComponent = entity->GetComponentAs<Graphics3DComponent>();
    Core::TransformComponent* currTransform = entity->GetComponentAs<Core::TransformComponent>();

/*    if (currGraphicsComponent)
      EDITOR->GetGFX()->GetDebugDrawer().BoundingBox(currGraphicsComponent->GetAABB(), EZ::Color::Cyan);
    else */if (currTransform)
    {
      EZ::float3 min = *(EZ::float3*)&(currTransform->GetPosition() - (currTransform->GetScale() * 0.5f));
      EZ::float3 max = *(EZ::float3*)&(currTransform->GetPosition() + (currTransform->GetScale() * 0.5f));

      EZ::AABB newAABB(min, max);
      EDITOR->GetGFX()->GetDebugDrawer().BoundingBox(newAABB, EZ::Color::Cyan);
    }
  }

  // Draws the selected Gizmo
  void EditorManager::DrawGizmo(void)
  {
    Core::Entity* entity = g_Factory.GetEntity(m_SelectedEntity);
    Core::TransformComponent* currTransform = entity->GetComponentAs<Core::TransformComponent>();

    // Checks whether the entity has a transform
    if (!currTransform)
      return;

    switch (m_CurrentGizmoType)
    {
      case Editor::GizmoType::None:
        EDITOR->GetGFX()->GetDebugDrawer().Axis(*((EZ::float3*)&currTransform->GetPosition()), GizmoSize);
        break;

      case Editor::GizmoType::Translate:
        EDITOR->GetGFX()->GetDebugDrawer().DrawGizmoTranslate(*((EZ::float3*)&currTransform->GetPosition()), GizmoSize);
        break;

      case Editor::GizmoType::Rotate:
        EDITOR->GetGFX()->GetDebugDrawer().DrawGizmoRotate(*((EZ::float3*)&currTransform->GetPosition()), GizmoSize);
        break;

      case Editor::GizmoType::Scale:
        EDITOR->GetGFX()->GetDebugDrawer().DrawGizmoScale(*((EZ::float3*)&currTransform->GetPosition()), GizmoSize);
        break;
    }
  }

  // Updates the position of the gizmo entities
  void EditorManager::UpdateGizmoPosition(void)
  {
    float dist = 18.0f;

    Core::TransformComponent* entityTransform = g_Factory.GetEntity(m_SelectedEntity)->GetComponentAs<Core::TransformComponent>();
    Core::TransformComponent* xTransform = m_CurrentGizmo[0]->GetComponentAs<Core::TransformComponent>();
    Core::TransformComponent* yTransform = m_CurrentGizmo[1]->GetComponentAs<Core::TransformComponent>();
    Core::TransformComponent* zTransform = m_CurrentGizmo[2]->GetComponentAs<Core::TransformComponent>();
    Core::TransformComponent* cTransform = m_CurrentGizmo[3]->GetComponentAs<Core::TransformComponent>();

    xTransform->SetPosition(entityTransform->GetPosition() + Math::Vector3(dist, 0.0f, 0.0f));
    yTransform->SetPosition(entityTransform->GetPosition() + Math::Vector3(0.0f, dist, 0.0f));
    zTransform->SetPosition(entityTransform->GetPosition() + Math::Vector3(0.0f, 0.0f, dist));
    cTransform->SetPosition(entityTransform->GetPosition() + Math::Vector3(0.0f, 0.0f, 0.0f));
  }

  // Gets input to know what Gizmo to draw
  void EditorManager::SelectGizmo(void)
  {
    // Works only if the Left Control Key is being held
    if (!Input::Keyboard::IsKeyHeld(Input::Keyboard::LeftControl))
      return;

    if (Input::Keyboard::IsKeyTriggered(Input::Keyboard::Q))
      m_CurrentGizmoType = GizmoType::None;

    else if (Input::Keyboard::IsKeyTriggered(Input::Keyboard::W))
      m_CurrentGizmoType = GizmoType::Translate;

    else if (Input::Keyboard::IsKeyTriggered(Input::Keyboard::E))
      m_CurrentGizmoType = GizmoType::Rotate;

    else if (Input::Keyboard::IsKeyTriggered(Input::Keyboard::R))
      m_CurrentGizmoType = GizmoType::Scale;
  }
}
