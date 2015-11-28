#include "Content.h" 
#include "../Filesystem/FileSystem.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Loads the default resources from the engine.
    @note  Currently only generating the first 128 characters of the ASCII
    character set.
    @todo  Figure out how to load both shader vertex and frag shaders into
    the one constructor dynamically.
    Figure out a way to find the folder paths dynamically.
    */
    /**************************************************************************/
    void Content::LoadCoreAssets()
    {
      DCTrace << "\n[Content::LoadDefaultResources] - Loading default resources \n";

      // In the future, find a way to dynamically find the folder paths?
      auto SpritePath = CoreAssetsPath + "Sprites/";
      auto SoundPath = CoreAssetsPath + "Sounds/";
      auto FontPath = CoreAssetsPath + "Fonts/";
      auto ShaderPath = CoreAssetsPath + "Shaders/";
      auto ArchetypePath = CoreAssetsPath + "Archetypes/";

      // Load default shaders
      AddShader(std::string("SpriteShader"), ShaderPtr(new Shader(std::string("SpriteShader"),
        ShaderPath + "SpriteShader.vs",
        ShaderPath + "SpriteShader.frag")));
      AddShader(std::string("SpriteTextShader"), ShaderPtr(new Shader(std::string("SpriteTextShader"),
        ShaderPath + "SpriteTextShader.vs",
        ShaderPath + "SpriteTextShader.frag")));
      AddShader(std::string("GUIShader"), ShaderPtr(new Shader(std::string("GUIShader"),
        ShaderPath + "GUIShader.vs",
        ShaderPath + "GUIShader.frag")));

      // Load shaders
      //std::vector<std::string> coreShaders;
      //if (!FileSystem::DirectoryExtractFilePaths(EngineInfo->ShaderPath, coreShaders))
      //  throw DCException("Content::LoadCoreAssets - Failed to load shader files!");
      //for (auto shader : coreShaders) {
      //  auto shaderName = FileSystem::FileExtractWithoutExtension(shader);
      //  AddShader(shaderName, ShaderPtr(new Shader(shaderName,
      //            EngineInfo->ShaderPath + "SpriteShader.vs", 
      //            EngineInfo->ShaderPath + "SpriteShader.frag")));
      //}

      // Load sprites
      ScanForSpriteSources(SpritePath);
      // Load sound files
      ScanForSoundCues(SoundPath);
      // Load default fonts      
      AddFont(std::string("Verdana"), FontPtr(new Font(FontPath + "Verdana.ttf")));
      DCTrace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";
      // Load archetypes
      ScanForArchetypes(ArchetypePath);
    }


    /**************************************************************************/
    /*!
    @brief Removes the specified resource from the Content system.
    @param resource A pointer to the specified resource.
    @note  The resource will be removed from the resource map to which it
    belongs and its dat file deleted.
    */
    /**************************************************************************/
    void Content::RemoveResource(ResourcePtr)
    {
    }

    void Content::ScanForLevels()
    {
      ScanForLevels(ProjectInfo->ProjectPath + ProjectInfo->ResourcePath + "Levels/");
    }

    void Content::ScanForArchetypes()
    {
      ScanForArchetypes(ProjectInfo->ProjectPath + ProjectInfo->ResourcePath + "Archetypes/");
    }

    void Content::ScanForSpriteSources()
    {
      ScanForSpriteSources(ProjectInfo->ProjectPath + ProjectInfo->AssetPath + "Images/");
    }

    void Content::ScanForSoundCues()
    {
      ScanForSoundCues(ProjectInfo->ProjectPath + ProjectInfo->AssetPath + "Sounds/");
    }

    /**************************************************************************/
    /*!
    @brief  Scans the Level Path for level files.
    */
    /**************************************************************************/
    void Content::ScanForLevels(std::string& levelPath)
    {
      DCTrace << "Content::ScanForLevels - Scanning for levels on the current project \n";
      //std::string LevelPath("Projects/Rebound/Resources/Levels/");

      // Load sound files
      std::vector<std::string> levels;
      if (!FileSystem::DirectoryListFilePaths(levelPath, levels))
        throw DCException("Content::ScanForLevels - Failed to load level files!");
      for (auto level : levels) {
        auto soundName = FileSystem::FileNoExtension(level);
        AddLevel(soundName, LevelPtr(new Level(level)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the specified path for archetype files.
    */
    /**************************************************************************/
    void Content::ScanForArchetypes(std::string& archetypePath)
    {
      DCTrace << "Content::ScanForArchetypes - Scanning for archetypes on the current project \n";
      //std::string ArchetypePath("Projects/Rebound/Resources/Archetypes/");

      // Load sound files
      std::vector<std::string> archetypes;
      if (!FileSystem::DirectoryListFilePaths(archetypePath, archetypes))
        throw DCException("Content::ScanForArchetypes - Failed to load archetype files!");
      for (auto archetype : archetypes) {
        auto archetypeName = FileSystem::FileNoExtension(archetype);
        AddArchetype(archetypeName, ArchetypePtr(new Archetype(archetype)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the specified path for SpriteSource files.
    */
    /**************************************************************************/
    void Content::ScanForSpriteSources(std::string & spriteSourcePath)
    {
      DCTrace << "Content::ScanForSpriteSources - Scanning for archetypes on the current project \n";

      // Load sprites
      std::vector<std::string> sprites;
      if (!FileSystem::DirectoryListFilePaths(spriteSourcePath, sprites))
        throw DCException("Content::LoadCoreAssets - Failed to load sprite files!");
      for (auto sprite : sprites) {
        auto spriteName = FileSystem::FileNoExtension(sprite);
        AddSpriteSource(spriteName, SpriteSourcePtr(new SpriteSource(sprite)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the specified path for SoundCue files.
    */
    /**************************************************************************/
    void Content::ScanForSoundCues(std::string & soundCuePath)
    {
      // Load sound files
      std::vector<std::string> soundCues;
      if (!FileSystem::DirectoryListFilePaths(soundCuePath, soundCues))
        throw DCException("Content::LoadCoreAssets - Failed to load sound files!");
      for (auto sound : soundCues) {
        auto soundName = FileSystem::FileNoExtension(sound);
        AddSoundCue(soundName, SoundCuePtr(new SoundCue(sound)));
      }
    }

    ProjectDataPtr & Content::ProjectSettings()
    {
      return ProjectInfo;
    }

  }
}