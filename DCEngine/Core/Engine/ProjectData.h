#pragma once
#include <string>
#include <memory>

namespace DCEngine {

  struct ProjectData {
    std::string ProjectName;
    unsigned int ResolutionWidth;
    unsigned int ResolutionHeight;
    std::string DefaultSpaceName;
    std::string DefaultLevelName;

    std::string AssetPath;
    std::string SpritePath;
    std::string ShaderPath;
    std::string FontPath;
    std::string ArchetypePath;
    std::string LevelPath;
  };

  struct EngineData {
    std::string ProjectName;
    unsigned int ResolutionWidth;
    unsigned int ResolutionHeight;

    std::string AssetPath;
    std::string SpritePath;
    std::string ShaderPath;
    std::string FontPath;
    std::string SoundPath;

    std::string ArchetypePath;
    std::string LevelPath;
  };

  using EngineDataPtr = std::unique_ptr<EngineData>;
  using ProjectDataPtr = std::unique_ptr<ProjectData>;

  struct AssetsData {
  };

  using AssetsDataPtr = std::unique_ptr<AssetsData>;

}