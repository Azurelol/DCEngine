/******************************************************************************/
/*!
@file   Data.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/1/2015
@brief  This file contains the data structures used in the engine
        and project configuration.
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
  @brief Configuration data for a Daisy Engine project.
  @note This structure is deserialized from a config file called "Project.dcp"
  */
  /**************************************************************************/
  struct ProjectData {
    std::string ProjectName;
    unsigned int ResolutionWidth;
    unsigned int ResolutionHeight;
    std::string DefaultSpaceName;
    std::string DefaultLevelName;
    std::string AssetPath;

    std::string SpritePath;
    std::string SoundPath;
    std::string ShaderPath;
    std::string FontPath;
    std::string ArchetypePath;
    std::string LevelPath;
  };
  using ProjectDataPtr = std::unique_ptr<ProjectData>;

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