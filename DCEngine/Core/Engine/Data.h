/******************************************************************************/
/*!
@file   Data.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/1/2015
@brief  This file contains the data structures used in the engine
        and project configuration.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <string>
#include <memory>

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief General configuration data for the Daisy Chain Engine.
  @note This structure is deserialized from a config file called "Daisy.cfg"
  */
  /**************************************************************************/
  struct EngineConfig {

    /* Window Settings */
    std::string Caption;
    double Version;
    unsigned int Framerate;
    unsigned int ResolutionWidth;
    unsigned int ResolutionHeight;
    bool IsFullScreen;  
    std::string AssetPath;
    bool EditorEnabled;
    std::string ProjectsPath;
    std::string RecentProject;
		unsigned MaxDrawLayers;

    void Serialize(Json::Value& root) {
    }

    void Deserialize(Json::Value& root) {
      Caption = root.get("Caption", "").asString();
      Version = root.get("Version", "").asDouble();
      Framerate = root.get("Framerate", "").asInt();
      ResolutionWidth = root.get("ResolutionWidth", "").asInt();
      ResolutionHeight = root.get("ResolutionHeight", "").asInt();
      IsFullScreen = root.get("IsFullScreen", "").asBool(); 
      AssetPath = root.get("AssetPath", "").asString();
      EditorEnabled = root.get("EditorEnabled", "").asBool();
      ProjectsPath = root.get("ProjectsPath", "").asString();
      RecentProject = root.get("RecentProject", "").asString();
			MaxDrawLayers = root.get("MaxDrawLayers", "").asInt();
      
      /*SpritePath = root.get("SpritePath", "").asString();
      SoundPath = root.get("SoundPath", "").asString();
      ShaderPath = root.get("ShaderPath", "").asString();
      FontPath = root.get("FontPath", "").asString();
      ArchetypePath = root.get("ArchetypePath", "").asString();
      LevelPath = root.get("LevelPath", "").asString();*/
    }
  };
  using EngineConfigPtr = std::unique_ptr<EngineConfig>;

  /**************************************************************************/
  /*!
  @struct EditorPreferences Preferences for the Editor.
  */
  /**************************************************************************/
  struct EditorPreferences {    



    void Serialize(Json::Value& root) {
    }

    void Deserialize(Json::Value& root) {
    }

  };



  /**************************************************************************/
  /*!
  @struct ProjectProperties Configuration data for a project.
  */
  /**************************************************************************/
  struct ProjectProperties {
    std::string ProjectName;
    std::string ProjectPath;
    // Levels
    std::string DefaultSpace;
    std::string DefaultLevel;
    // Resources
    std::string ResourcePath;
    std::string AssetPath;
    // Settings
    bool Play;

    void Serialize(Json::Value& root) {
      root["Project Name"] = ProjectName;
      root["Project Path"] = ProjectPath;
      root["Default Space"] = DefaultSpace;
      root["Default Level"] = DefaultLevel;
      root["Resource Path"] = ResourcePath;
      root["Asset Path"] = AssetPath;
      root["Project Name"] = ProjectName;
      root["Play"] = Play;
    }
    void Deserialize(Json::Value& root) {

      ProjectName    = root.get("Project Name", "").asString();
      DefaultLevel   = root.get("Default Level", "").asString();
      DefaultSpace   = root.get("Default Space", "").asString();
      ProjectPath    = root.get("Project Path", "").asString();
      ResourcePath   = root.get("Resource Path", "").asString();
      AssetPath      = root.get("Asset Path", "").asString();
      Play           = root.get("Play", "").asBool();
    }

    static std::string Extension() { return ".dceproj"; }

  };
  using ProjectDataPtr = std::unique_ptr<ProjectProperties>;

  struct AssetsData {
    std::string SpritePath;
    std::string SoundPath;
    std::string ShaderPath;
    std::string FontPath;
    std::string ArchetypePath;
    std::string LevelPath;

  };
  using AssetsDataPtr = std::unique_ptr<AssetsData>;

}