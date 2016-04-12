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
        
    std::string Caption;
    double Version;

    void Serialize(Json::Value& root) {
    }

    void Deserialize(Json::Value& root) {
      Caption = root.get("Caption", "").asString();
      Version = root.get("Version", "").asDouble();
    }
  };
  using EngineConfigPtr = std::unique_ptr<EngineConfig>;

  struct EngineStatistics {
    float RunTime;

    EngineStatistics() : RunTime(0) {}
  };

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