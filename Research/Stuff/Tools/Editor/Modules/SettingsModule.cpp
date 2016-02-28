/******************************************************************************/
/*
  File:    SettingsModule.cpp
  Author:  Alejandro Hitti
  Date:    11/25/14
  Brief:   The Settings Module lets the user modify certain aspects of the
           editor's functionality and appearance.

  All content © 2014-2015 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

// Includes
//------------------------------------------------------------------------------
#include "stdinc.h"

#include "SettingsModule.h"
#include "Colors.h"

namespace Editor
{
  namespace Styles
  {
    // Generates an array of strings with the enums as const char*
    #define STYLE(x) #x,
    const char* StyleStrings[] = 
    {
      #include "StylesDeclaration.h"
      "Invalid"
    };
    #undef STYLE
  }

  bool SettingsModule::m_IsActive = false;
  bool SettingsModule::m_IsVisible = true;
  ImGuiStyle SettingsModule::m_Style = ImGuiStyle();
  bool SettingsModule::m_IsFirstTime = true;

  // Initializes the Module
  bool SettingsModule::SettingsModuleInitialize()
  {
    m_Style.WindowRounding = 5.0f;
    ChangeStyle(Styles::Pumpkin);
    ImGui::GetStyle() = m_Style;

    return true;
  }

  // Checks if the module is currently active
  bool SettingsModule::SettingsModuleIsActive()
  {
    return m_IsActive;
  }

  // Sets the module active flag to the state passed in
  void SettingsModule::SettingsModuleSetActive(bool state)
  {
    m_IsActive = state;
  }

  // Checks if the module is currently visible
  bool SettingsModule::SettingsModuleIsVisible()
  {
    return m_IsVisible;
  }

  // Sets the module visible flag to the state passed in
  void SettingsModule::SettingsModuleSetVisible(bool state)
  {
    m_IsVisible = state;
  }

  // Updates the module
  void SettingsModule::SettingsModuleUpdate()
  {
    if (!m_IsVisible || !m_IsActive)
      return;

    ImGui::Begin("Settings", &m_IsActive, ImVec2(0, 0), EDITOR->GetFlags().fillAlpha, EDITOR->GetLayoutFlags());

    // Positions the window the first time you run it
    if (m_IsFirstTime)
    {
      EDITOR->SetModulePosition((int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y, Editor::RefPoint::BottomRight, -5, -5);
      m_IsFirstTime = false;
      ImGui::End();
      return;
    }

    if(ImGui::CollapsingHeader("Appeareance", (const char*)0, true, true))
    {
      static int style = Styles::Style::Pumpkin;
      if (ImGui::Combo("Choose a Style", &style, Styles::StyleStrings, Styles::Style::NumStyles))
      {
        SetHelpTooltip("Change the editor's theme.");
        ChangeStyle((Styles::Style)style);
      }

      ChangeWindowOptions();
      ChangeSizes();

      ImGui::GetStyle() = m_Style;

      //SaveStyle();
    }

    if (ImGui::CollapsingHeader("Options", (const char*)0, true, true))
      ShowOptions();

    ImGui::End();
  }

  // Changes the style to the specified one
  void SettingsModule::ChangeStyle(Styles::Style style)
  {
    switch (style)
    {
      case Editor::Styles::Default:
        m_Style.Colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
        m_Style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        m_Style.Colors[ImGuiCol_Border]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        m_Style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
        m_Style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);   // Background of checkbox, radio button, plot, slider, text input
        m_Style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.50f, 0.50f, 1.00f, 0.45f);
        m_Style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
        m_Style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.40f, 0.40f, 0.80f, 0.15f);
        m_Style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
        m_Style.Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
        m_Style.Colors[ImGuiCol_CheckHovered]         = ImVec4(0.60f, 0.40f, 0.40f, 0.45f);
        m_Style.Colors[ImGuiCol_CheckActive]          = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
        m_Style.Colors[ImGuiCol_SliderGrab]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
        m_Style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
        m_Style.Colors[ImGuiCol_Button]               = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
        m_Style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
        m_Style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
        m_Style.Colors[ImGuiCol_Header]               = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
        m_Style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
        m_Style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.60f, 0.60f, 0.80f, 1.00f);
        m_Style.Colors[ImGuiCol_Column]               = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        m_Style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.60f, 0.40f, 0.40f, 1.00f);
        m_Style.Colors[ImGuiCol_ColumnActive]         = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
        m_Style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
        m_Style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
        m_Style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
        m_Style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.50f, 0.50f, 0.90f, 0.50f);
        m_Style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
        m_Style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
        m_Style.Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        m_Style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        m_Style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
        m_Style.Colors[ImGuiCol_TooltipBg]            = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
        break;

      case Editor::Styles::Monokai:
        m_Style.Colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.90f,            1.00f);
        m_Style.Colors[ImGuiCol_WindowBg]             = ImVec4(Colors::Monokai::BurnedClay,    1.00f);
        m_Style.Colors[ImGuiCol_Border]               = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f,            0.60f);
        m_Style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.80f, 0.80f, 0.80f,            0.30f);
        m_Style.Colors[ImGuiCol_TitleBg]              = ImVec4(Colors::Monokai::DarkBlue,      0.45f);
        m_Style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(Colors::Monokai::DarkBlue,      0.20f);
        m_Style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(Colors::Monokai::DarkBlue,      0.15f);
        m_Style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(Colors::Monokai::PastelBlue,    0.30f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(Colors::Monokai::BrightBlue,    0.40f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(Colors::Monokai::PastelGreen,   0.40f);
        m_Style.Colors[ImGuiCol_ComboBg]              = ImVec4(Colors::Monokai::DarkBlue,      0.99f);
        m_Style.Colors[ImGuiCol_CheckHovered]         = ImVec4(Colors::Monokai::DarkGreen,     0.45f);
        m_Style.Colors[ImGuiCol_CheckActive]          = ImVec4(0.90f, 0.90f, 0.90f,            0.50f);
        m_Style.Colors[ImGuiCol_SliderGrab]           = ImVec4(Colors::Monokai::PastelGreen,   0.30f);
        m_Style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(Colors::Monokai::PastelGreen,   1.00f);
        m_Style.Colors[ImGuiCol_Button]               = ImVec4(Colors::Monokai::DarkGreen,     0.60f);
        m_Style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(Colors::Monokai::PastelGreen,   0.70f);
        m_Style.Colors[ImGuiCol_ButtonActive]         = ImVec4(Colors::Monokai::BrightGreen,   0.70f);
        m_Style.Colors[ImGuiCol_Header]               = ImVec4(Colors::Monokai::DarkBlue,      0.45f);
        m_Style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(Colors::Monokai::DarkGreen,     0.80f);
        m_Style.Colors[ImGuiCol_HeaderActive]         = ImVec4(Colors::Monokai::PastelGreen,   1.00f);
        m_Style.Colors[ImGuiCol_Column]               = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.60f, 0.40f, 0.40f,            1.00f);
        m_Style.Colors[ImGuiCol_ColumnActive]         = ImVec4(0.80f, 0.50f, 0.50f,            1.00f);
        m_Style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f,            0.30f);
        m_Style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f,            0.60f);
        m_Style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f,            0.90f);
        m_Style.Colors[ImGuiCol_CloseButton]          = ImVec4(Colors::Monokai::PastelBlue,    0.50f);
        m_Style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(Colors::Monokai::BrightBlue,    0.60f);
        m_Style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(Colors::Monokai::PastelGreen,   1.00f);
        m_Style.Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f,            1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(Colors::Monokai::BrightOrange,  1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(Colors::Monokai::PaleOrange,    1.00f);
        m_Style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f,            0.35f);
        m_Style.Colors[ImGuiCol_TooltipBg]            = ImVec4(0.05f, 0.05f, 0.10f,            0.90f);
        break;

      case Editor::Styles::Pumpkin:
        m_Style.Colors[ImGuiCol_Text]                 = ImVec4(Colors::Pumpkin::WhiteOrange,   1.00f);
        m_Style.Colors[ImGuiCol_WindowBg]             = ImVec4(Colors::Pumpkin::Bye,           1.00f);
        m_Style.Colors[ImGuiCol_Border]               = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f,            0.60f);
        m_Style.Colors[ImGuiCol_FrameBg]              = ImVec4(Colors::Pumpkin::LightBrown,    0.30f);
        m_Style.Colors[ImGuiCol_TitleBg]              = ImVec4(Colors::Pumpkin::DarkOrange,    0.45f);
        m_Style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(Colors::Pumpkin::DarkOrange,    0.20f);
        m_Style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(Colors::Pumpkin::DarkOrange,    0.15f);
        m_Style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(Colors::Pumpkin::Orange,        0.30f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(Colors::Pumpkin::PaleOrange,    0.40f);
        m_Style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(Colors::Pumpkin::YellowOrange,  0.40f);
        m_Style.Colors[ImGuiCol_ComboBg]              = ImVec4(Colors::Pumpkin::LightBrown,    0.99f);
        m_Style.Colors[ImGuiCol_CheckHovered]         = ImVec4(Colors::Pumpkin::PaleOrange,    0.45f);
        m_Style.Colors[ImGuiCol_CheckActive]          = ImVec4(Colors::Pumpkin::YellowOrange,  0.50f);
        m_Style.Colors[ImGuiCol_SliderGrab]           = ImVec4(Colors::Pumpkin::PaleOrange,    0.30f);
        m_Style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(Colors::Pumpkin::YellowOrange,  1.00f);
        m_Style.Colors[ImGuiCol_Button]               = ImVec4(Colors::Pumpkin::DarkOrange,    0.60f);
        m_Style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(Colors::Pumpkin::Orange,        1.00f);
        m_Style.Colors[ImGuiCol_ButtonActive]         = ImVec4(Colors::Pumpkin::PaleOrange,    1.00f);
        m_Style.Colors[ImGuiCol_Header]               = ImVec4(Colors::Pumpkin::DarkOrange,    0.45f);
        m_Style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(Colors::Pumpkin::Orange,        0.80f);
        m_Style.Colors[ImGuiCol_HeaderActive]         = ImVec4(Colors::Pumpkin::PaleOrange,    1.00f);
        m_Style.Colors[ImGuiCol_Column]               = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.60f, 0.40f, 0.40f,            1.00f);
        m_Style.Colors[ImGuiCol_ColumnActive]         = ImVec4(0.80f, 0.50f, 0.50f,            1.00f);
        m_Style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f,            0.30f);
        m_Style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f,            0.60f);
        m_Style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f,            0.90f);
        m_Style.Colors[ImGuiCol_CloseButton]          = ImVec4(Colors::Pumpkin::DarkOrange,    0.50f);
        m_Style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(Colors::Pumpkin::Orange,        0.60f);
        m_Style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(Colors::Pumpkin::PaleOrange,    1.00f);
        m_Style.Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f,            1.00f);
        m_Style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f,            1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(Colors::Pumpkin::YellowOrange,  1.00f);
        m_Style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(Colors::Pumpkin::PaleOrange,    1.00f);
        m_Style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f,            0.35f);
        m_Style.Colors[ImGuiCol_TooltipBg]            = ImVec4(0.05f, 0.05f, 0.10f,            0.90f);
        break;

      default:
        break;
    }
  }

  void SettingsModule::ChangeSizes(void)
  {
    ImGui::SliderFloat("Alpha", &m_Style.Alpha, 0.20f, 1.0f, "%.2f");                 // Not exposing zero here so user doesn't "lose" the UI. But application code could have a toggle to switch between zero and non-zero.
    SetHelpTooltip("Change the overall editor's alpha value.");
    //ImGui::SliderFloat2("WindowPadding", (float*)&m_Style.WindowPadding, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat("WindowRounding", &m_Style.WindowRounding, 0.0f, 16.0f, "%.0f");
    SetHelpTooltip("Change the roundness of the editor's edges.");
    ImGui::SliderFloat2("FramePadding", (float*)&m_Style.FramePadding, 0.0f, 20.0f, "%.0f");
    SetHelpTooltip("Change the distance between objects in the editor.");
    ImGui::SliderFloat2("ItemSpacing", (float*)&m_Style.ItemSpacing, 0.0f, 20.0f, "%.0f");
    SetHelpTooltip("Change the spacing between objects in the editor.");
    //ImGui::SliderFloat2("ItemInnerSpacing", (float*)&m_Style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
    //ImGui::SliderFloat2("TouchExtraPadding", (float*)&m_Style.TouchExtraPadding, 0.0f, 20.0f, "%.0f");
    //ImGui::SliderFloat("TreeNodeSpacing", &m_Style.TreeNodeSpacing, 0.0f, 20.0f, "%.0f");
    //ImGui::SliderFloat("ScrollBarWidth", &m_Style.ScrollBarWidth, 0.0f, 20.0f, "%.0f");
  }

  // Changes window options to alter the editor
  void SettingsModule::ChangeWindowOptions(void)
  {
    //ImGui::Checkbox("no titlebar", &EDITOR->GetFlags().noTitlebar); ImGui::SameLine(150);
    //ImGui::Checkbox("no border", &EDITOR->GetFlags().noBorder); ImGui::SameLine(300);
    //ImGui::Checkbox("no resize", &EDITOR->GetFlags().noResize);
    //ImGui::Checkbox("no move", &EDITOR->GetFlags().noMove); ImGui::SameLine(150);
    //ImGui::Checkbox("no scrollbar", &EDITOR->GetFlags().noScrollbar);
    ImGui::SliderFloat("Fill Alpha", &EDITOR->GetFlags().fillAlpha, 0.0f, 1.0f);
    SetHelpTooltip("Change the editor's background opacity.");
  }

  // Saves style into a file. If not found, selects default style.
  void SettingsModule::SaveStyle(void)
  {
    static int dt = 120;

    if (ImGui::Button("Save Style", ImVec2(100, 0)))
    {
      dt = 0;
    }
    SetHelpTooltip("Save current appearance settings.");

    if (dt < 60)
    {
      ImGui::SameLine();
      ImGui::Text("Your style has been saved!");
    }
    else if (dt < 120)
    {
      ImGui::SameLine();
      ImVec4 col = m_Style.Colors[ImGuiCol_Text];
      col.w -= dt / 80.0f;
      ImGui::TextColored(col, "Your style has been saved!");
    }

    ++dt;
  }

  // Displays the options collapsable menu
  void SettingsModule::ShowOptions(void)
  {
    // Fullscreen checkbox
    static bool isFullscreen = EDITOR->GetGFX()->GetFullscreen();
    if (ImGui::Checkbox("FullScreen", &isFullscreen))
      EDITOR->GetGFX()->SetFullscreen(isFullscreen);
    SetHelpTooltip("Toggle between FullScreen/Windowed.");

    ImGui::SameLine();

    // Wireframe mode
    static bool isWireframe = false;
    if (ImGui::Checkbox("Wireframes", &isWireframe))
    {
      if (isWireframe)
        EDITOR->GetGFX()->SetFillMode(EZ::FILL_MODE_WIREFRAME);
      else
        EDITOR->GetGFX()->SetFillMode(EZ::FILL_MODE_SOLID);
    }
    SetHelpTooltip("Toggle between Solid/Wireframe fill modes.");

    // Sound pause
    static bool isSoundPaused = false;
    if (ImGui::Checkbox("Mute Sound", &isSoundPaused))
    {
      if (isSoundPaused)
        g_Messaging.BroadcastMessage(Core::MessageName::PauseAudio, nullptr, nullptr);
      else
        g_Messaging.BroadcastMessage(Core::MessageName::ResumeAudio, nullptr, nullptr);
    }
    SetHelpTooltip("Mutes the music and all SFX currently playing.");

    ImGui::SameLine();

    // Pause/Unpause game
    bool isGamePaused = EDITOR->IsGamePaused();
    if (ImGui::Checkbox("Game Pause", &isGamePaused))
    {
      if (isGamePaused)
        g_Messaging.BroadcastMessage(Core::MessageName::PauseGame, nullptr, nullptr);
      else
        g_Messaging.BroadcastMessage(Core::MessageName::UnpauseGame, nullptr, nullptr);
    }

    // NOT WORKING
    // Change resolution
    //static auto resolutions = EDITOR->GetGFX()->GetDisplayModeList();
    //std::string resolutionsStrings = "";

    //for (unsigned i = 0; i < resolutions.size(); ++i)
    //  resolutionsStrings += std::to_string((int)resolutions[i].resolution.width) + " x " + std::to_string((int)resolutions[i].resolution.height) + '\0';
    //


    //static int index = 0;

    //if (ImGui::Combo("Change Resolution", &index, resolutionsStrings.c_str()))
    //{
    //  EDITOR->GetGFX()->SetDisplayMode(index);
    //}

  }
}
