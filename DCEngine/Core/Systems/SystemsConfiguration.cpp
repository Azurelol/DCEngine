/*****************************************************************************/
/*!
\file   SystemConfiguration.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/3/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "SystemsConfiguration.h"

#include "../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    ConfigurationFiles & ConfigurationFiles::Access()
    {
      return Daisy->Configuration();
    }

    /**************************************************************************/
    /*!
    @brief Configuration constructor.
    @param extension The name of the configuration.
    */
    /**************************************************************************/
    Configuration::Configuration(std::string name) : FileName("Configuration" + name + ".cfg")
    {
    }

    /**************************************************************************/
    /*!
    @brief Loads the configuration from the file.
    @param filePath The path to the file.
    */
    /**************************************************************************/
    bool Configuration::Load(std::string filePath)
    {
      std::string configData;

      // Attempt to load the Graphics Config
      if (FileSystem::FileReadToString(filePath, configData)) {
        Serialization::Deserialize(this, configData);
        DCTrace << "Engine::LoadConfigurationFiles - Successfully loaded '" << filePath << "'! \n";
        return true;
      }
      else {
        Serialization::Serialize(this, configData);
        FileSystem::FileWriteString(filePath, configData);
        DCTrace << "Engine::LoadConfigurationFiles - Failed to load '" << filePath << "'! \n";
      }
      return false;
    }

    bool Configuration::Load()
    {
      return Load(FileName);
    }

    /**************************************************************************/
    /*!
    @brief Saves the configuration to the specified file.
    @param filePath The path which to save the configuration at.
    */
    /**************************************************************************/
    void Configuration::Save(std::string filePath)
    {
      std::string configData;
      Serialization::Serialize(*this, configData);
      FileSystem::FileWriteString(filePath, configData);
    }

    bool Configuration::Save()
    {
      Save(FileName);
      return true; // lol
    }

    /*============
      GUI CONFIG
    ============*/
    GUIConfig::GUIConfig() : Configuration("GUI"), Style(ImGui::GetStyle()), 
      FontPath(boost::filesystem::initial_path().string() + "\\Dependencies\\include\IMGUI\\extra_fonts\\DroidSans.ttf")
    {
    }

    /**************************************************************************/
    /*!
    @struct Serializes GUIConfig Configuration.
    @param root A reference to the JSON root.
    */
    /**************************************************************************/
    void GUIConfig::Serialize(Json::Value & root)
    {
      // Sizes
      DCE_JSON_SERIALIZE(Style.Alpha);
      DCE_JSON_SERIALIZE(Style.WindowRounding);
      DCE_JSON_SERIALIZE(Style.FrameRounding);
      DCE_JSON_SERIALIZE(Style.WindowFillAlphaDefault);
      DCE_JSON_SERIALIZE(Style.ColumnsMinSpacing);
      DCE_JSON_SERIALIZE(Style.ScrollbarSize);
      DCE_JSON_SERIALIZE(Style.ScrollbarRounding);
      DCE_JSON_SERIALIZE(Style.GrabMinSize);
      DCE_JSON_SERIALIZE(Style.GrabRounding);
      DCE_JSON_SERIALIZE(Style.AntiAliasedLines);
      DCE_JSON_SERIALIZE(Style.AntiAliasedShapes);
      // Colors
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Text]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Text]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TextDisabled]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_WindowBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ChildWindowBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Border]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_BorderShadow]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBgHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBgActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBgCollapsed]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBgActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_MenuBarBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrab]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrabHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrabActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ComboBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_CheckMark]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_SliderGrab]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_SliderGrabActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Button]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ButtonHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ButtonActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Header]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_HeaderHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_HeaderActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_Column]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ColumnHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ColumnActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGrip]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGripHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGripActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButton]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButtonHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButtonActive]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotLines]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotLinesHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotHistogram]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotHistogramHovered]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TextSelectedBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_TooltipBg]);
      DCE_JSON_SERIALIZE_VEC4(Style.Colors[ImGuiCol_ModalWindowDarkening]);

    }

    /**************************************************************************/
    /*!
    @struct Deserializes GUIConfig Configuration.
    @param root A reference to the JSON root.
    */
    /**************************************************************************/
    void GUIConfig::Deserialize(Json::Value & root)
    {
      // Sizes
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.Alpha).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.WindowRounding).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.FrameRounding).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.WindowFillAlphaDefault).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.ColumnsMinSpacing).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.ScrollbarSize).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.ScrollbarRounding).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.GrabMinSize).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.GrabRounding).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.AntiAliasedLines).asFloat();
      DCE_JSON_DESERIALIZE_INTRINSIC(Style.AntiAliasedShapes).asFloat();
      // Colors
      // Colors
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Text]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Text]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TextDisabled]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_WindowBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ChildWindowBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Border]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_BorderShadow]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBgHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_FrameBgActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBgCollapsed]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TitleBgActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_MenuBarBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrab]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrabHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ScrollbarGrabActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ComboBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_CheckMark]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_SliderGrab]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_SliderGrabActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Button]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ButtonHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ButtonActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Header]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_HeaderHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_HeaderActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_Column]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ColumnHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ColumnActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGrip]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGripHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ResizeGripActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButton]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButtonHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_CloseButtonActive]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotLines]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotLinesHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotHistogram]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_PlotHistogramHovered]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TextSelectedBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_TooltipBg]);
      DCE_JSON_DESERIALIZE_VEC4(Style.Colors[ImGuiCol_ModalWindowDarkening]);
    }


    EditorConfig::EditorConfig(): Configuration("Editor"), 
      AutoSave(false), AutoSaveTime(60), GridActive(true), GridLength(1.0f), GridColor(0.3f, 0.3f, 0.3f, 0.2f),
      LockedColor(1.0f, 0.0f, 0.0f, 1.0f), Snapping(true), SnapDistance(1.0f), SnapAngle(15.0f),
      AutoSaveTimer(AutoSaveTime, Time::Timer::Mode::Countdown, true),
      CameraLastPos(0.0f, 0.0f, 40.0f), ExternalTextEditor("Notepad++"), TextEditorOpenExternally(false), 
      CompilationPopUps(false), CompileOnContextSwitch(false)
    {
    }

		GraphicsConfig::GraphicsConfig() : Configuration("Graphics"),
			MaxDrawLayers(5), ScreenWidth(1440), ScreenHeight(900), Framerate(60), Fullscreen(false),
			Caption("Daisy Chain Engine"), ClearColor(0.0f, 0.5f, 1.0f, 1.0f), ViewportScale(1.0f, 1.0f),
			ViewportRatio(1.0f, 1.0f), Exposure(.5f), Samples(4)
    {
    }

    AudioConfig::AudioConfig() : Configuration("Audio"), Enabled(true), MasterVolume(100)
    {
    }

    DebugConfig::DebugConfig() : Configuration("Debug")
    {
    }

    void DebugConfig::Serialize(Json::Value & root)
    {
    }

    void DebugConfig::Deserialize(Json::Value & root)
    {
    }

}
}
