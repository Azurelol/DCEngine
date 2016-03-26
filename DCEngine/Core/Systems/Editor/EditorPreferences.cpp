/******************************************************************************/
/*!
@file   EditorPreferences.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/4/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "EditorPreferences.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
        
    /**************************************************************************/
    /*!
    \brief  EditorPreferences constructor.
    */
    /**************************************************************************/
    EditorPreferences::EditorPreferences() : EditorModule(true)
    {
    }

    /**************************************************************************/
    /*!
    \brief  EditorPreferences destructor.
    */
    /**************************************************************************/
    EditorPreferences::~EditorPreferences()
    {
    }

    void EditorPreferences::Update()
    {
    }
    
    /**************************************************************************/
    /*!
    \brief  Displays the EditorPreferences window.
    */
    /**************************************************************************/
    void EditorPreferences::Display()
    {
      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiSetCond_Always);
      ImGui::Begin("Preferences", &WindowEnabled, ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize);
      DisplayColumns();



      ImGui::End();
    }

    /**************************************************************************/
    /*!
    \brief  Displays the current tab.
    */
    /**************************************************************************/
    void EditorPreferences::DisplayTab()
    {
      switch (CurrentTab) {
      case Tab::Editor:
        TabEditor();
        break;
      case Tab::Graphics:
        TabGraphics();
        break;
      case Tab::Audio:
        TabAudio();
        break;
      case Tab::Debug:
        TabDebug();
        break;
      case Tab::Style:
        TabStyle();
        break;
      case Tab::StyleSizes:
        TabStyleSizes();
        break;
      case Tab::StyleColors:
        TabStyleColors();
        break;
      default:
        break;
      }
    }

    /**************************************************************************/
    /*!
    \brief  Displays the columns.
    */
    /**************************************************************************/
    void EditorPreferences::DisplayColumns()
    {
      ImGui::BeginGroup();
      
      ImGui::Columns(2, "WindowPreferences");
      // Left Column
      if (ImGui::Selectable("Editor")) CurrentTab = Tab::Editor;
      if (ImGui::Selectable("Graphics")) CurrentTab = Tab::Graphics;
      if (ImGui::Selectable("Audio")) CurrentTab = Tab::Audio;
      if (ImGui::Selectable("Debug")) CurrentTab = Tab::Debug;
      if (ImGui::Selectable("Style")) CurrentTab = Tab::Style;

      // Bottom
      // Saves before exiting
      ImGui::Text("");
      if (ImGui::Button("OK")) {
        Save();
        WindowEnabled = false;
      }
      // Exits without saving
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        WindowEnabled = false;
      }

      // Right Column
      ImGui::NextColumn();
      DisplayTab();
      ImGui::EndGroup();
    }

    /**************************************************************************/
    /*!
    \brief  Displays the Editor's preferences.
    */
    /**************************************************************************/
    void EditorPreferences::TabEditor()
    {
      static auto& config = ConfigurationFiles::Access().Editor;
      ImGui::InputInt("AutoSaveTime", &config.AutoSaveTime);
      ImGui::Checkbox("GridActive", &config.GridActive);
      ImGui::SliderFloat("GridLength", &config.GridLength, 1.0f, 10.0f);
      ImGui::SliderFloat4("GridColor", &config.GridColor[0], 0.0f, 1.0f);
      ImGui::Checkbox("Snapping", &config.Snapping);
      ImGui::InputFloat("SnapDistance", &config.SnapDistance, 0.25f);
      ImGui::InputFloat("SnapAngle", &config.SnapAngle, 1.0f);
      ImGui::SliderFloat4("LockedColor", &config.LockedColor[0], 0.0f, 1.0f);

      const char* editors[] = { "Notepad++", "Sublime" };
      static int editorSelected = 0;
      // What a hack
      if (config.ExternalTextEditor == editors[1])
        editorSelected = 1;
      if (ImGui::Combo("ExternalTextEditor", &editorSelected, editors, IM_ARRAYSIZE(editors))) {
        config.ExternalTextEditor = editors[editorSelected];
      }
      ImGui::Checkbox("CompileOnContextSwitch", &config.CompileOnContextSwitch);
    }

    /**************************************************************************/
    /*!
    \brief  Displays the engine's debugging preferences.
    */
    /**************************************************************************/
    void EditorPreferences::TabDebug()
    {
      static auto& config = ConfigurationFiles::Access().Debug;
    }

    /**************************************************************************/
    /*!
    \brief  Displays the GUI Style's preferences.
    */
    /**************************************************************************/
    void EditorPreferences::TabStyle()
    {
      ImGui::ShowStyleEditor();
      //static auto& style = ConfigurationFiles::Access().GUI.Style;
      //ImGui::SliderFloat("Alpha", &style.Alpha, 0.0f, 1.0f);
    }

    void EditorPreferences::TabStyleSizes()
    {
      static auto& style = ConfigurationFiles::Access().GUI.Style;
      ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 1.0f, 16.0f);
      ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 1.0f, 16.0f);
      ImGui::SliderFloat("ChildWindowRounding", &style.ChildWindowRounding, 1.0f, 16.0f);
      //ImGui::SliderFloat2("FramePadding", &style.FramePadding[0], 1.0f, 20.0f);
      //ImGui::SliderInt2("ItemSpacing", &style.ItemSpacing[0], 1.0f, 20.0f);
    }

    void EditorPreferences::TabStyleColors()
    {
      static auto& style = ConfigurationFiles::Access().GUI.Style;
      GUI::SliderFloat4("Column Text", style.Colors[ImGuiCol_Text], 0.0f, 0.1f);
    }

    /**************************************************************************/
    /*!
    \brief  Displays the Graphics's preferences.
    */
    /**************************************************************************/
    void EditorPreferences::TabGraphics()
    {
      static auto& config = ConfigurationFiles::Access().Graphics;
      //float vec2f[2] = { config.ScreenWidth, config.ScreenHeight };
      //if (ImGui::InputFloat2("Resolution", vec2f, 3)) {
      //  config.ScreenWidth = vec2f[0]; config.ScreenHeight = vec2f[1];
      //}
      ImGui::InputInt("Width", &config.ScreenWidth, 10); 
      ImGui::InputInt("Height", &config.ScreenHeight, 10);
      ImGui::InputInt("Framerate", &config.Framerate, 1);
      ImGui::InputInt("Max Draw Layers", &config.MaxDrawLayers, 1);
			ImGui::SliderFloat("Exposure", &config.Exposure, 0, 3);
      ImGui::Checkbox("Fullscreen", &config.Fullscreen);
      ImGui::SliderFloat4("Clear Color", &config.ClearColor[0], 0.0f, 1.0f);
    }

    /**************************************************************************/
    /*!
    \brief  Displays the Audio's preferences.
    */
    /**************************************************************************/
    void EditorPreferences::TabAudio()
    {
      static auto& config = ConfigurationFiles::Access().Audio;
      ImGui::Checkbox("Enabled", &config.Enabled);
      ImGui::InputInt("Master Volume", &config.MasterVolume, 1);
    }

    void EditorPreferences::Save()
    {
      DispatchSystemEvents::EngineSaveConfigurations();
    }


  }
}