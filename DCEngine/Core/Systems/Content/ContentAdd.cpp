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
    @brief Adds a Font to the font resource map.
    @param The name of the Font.
    @param The pointer to the Font resource.
    @note  We load the font immediately so its ready for use by SpriteText
    component.
    */
    /**************************************************************************/
    void Content::AddFont(std::string & fontName, FontPtr fontPtr)
    {
      FontMap.insert(std::pair<std::string, FontPtr>(fontName, fontPtr));
      DCTrace << "Content::AddFont - " << fontName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds an archetype to the Archetype resource map.
    @param The name of the archetype.
    @param The pointer to the archetype.
    @note  Perhaps it's okay to overwrite previous archetypes, unlike
    other resources.
    */
    /**************************************************************************/
    void Content::AddArchetype(std::string & archetypeName, ArchetypePtr archetypePtr)
    {
      // Overwrite archetypes
      if (ArchetypeMap.count(archetypeName)) {
        ArchetypeMap.erase(archetypeName);
        DCTrace << "Content::AddArchetype - " << archetypeName << " is already present in the map. Overwriting \n";
        //return;
      }

      ArchetypeMap.insert(std::pair<std::string, ArchetypePtr>(archetypeName, archetypePtr));
      DCTrace << "Content::AddArchetype - " << archetypeName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a shader to the shader resource map.
    @param The name of the shader.
    @param The pointer to the shader resource.
    */
    /**************************************************************************/
    void Content::AddShader(std::string & shaderName, ShaderPtr shaderPtr)
    {
      ShaderMap.insert(std::pair<std::string, ShaderPtr>(shaderName, shaderPtr));
      DCTrace << "Content::AddShader - " << shaderName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a spritesource to the spritesource resource map.
    @param The name of the spritesource.
    @param The pointer to the spritesource resource.
    */
    /**************************************************************************/
    void Content::AddSpriteSource(std::string & spriteSourceName, SpriteSourcePtr spriteSourcePtr)
    {
      // Prevent duplicates
      if (SpriteSourceMap.count(spriteSourceName)) {
        DCTrace << "Content::AddSpriteSource - " << spriteSourceName << " is already present in the map.\n";
        return;
      }

      SpriteSourceMap.insert(std::pair<std::string, SpriteSourcePtr>(spriteSourceName, spriteSourcePtr));
      DCTrace << "Content::AddSpriteSource - " << spriteSourceName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a soundcue resource to the soundcue resource map.
    @param The name of the soundcue.
    @param The pointer to the soundcue resource.
    */
    /**************************************************************************/
    void Content::AddSoundCue(std::string & soundCueName, SoundCuePtr soundcuePtr)
    {
      // Prevent duplicates
      if (SoundCueMap.count(soundCueName)) {
        DCTrace << "Content::AddSoundCue - " << soundCueName << " is already present in the map.\n";
        return;
      }

      SoundCueMap.insert(std::pair<std::string, SoundCuePtr>(soundCueName, soundcuePtr));
      DCTrace << "Content::AddSoundCue - " << soundCueName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a level resource to the Level resource map.
    @param The name of the level.
    @param The pointer to the level resource.
    */
    /**************************************************************************/
    void Content::AddLevel(std::string & levelName, LevelPtr levelPtr)
    {
      // Prevent duplicates
      if (LevelMap.count(levelName)) {
        // Overwrite the current level
        LevelMap.erase(levelName);        
        DCTrace << "Content::AddLevel - " << levelName << " is already present in the map. Overwriting. \n";
        //return;
      }

      LevelMap.insert(std::pair<std::string, LevelPtr>(levelName, levelPtr));
      DCTrace << "Content::AddLevel - " << levelName << " was added.\n";
    }


    /**************************************************************************/
    /*!
    @brief Removes the specified resource from the Content system.
    @param resource A pointer to the specified resource.
    @note  The resource will be removed from the resource map to which it
    belongs and its dat file deleted.
    */
    /**************************************************************************/
    void Content::RemoveResource(ResourcePtr resource)
    {
      // Level
      if (dynamic_cast<Level*>(resource)) {
        // Delete it from file

        // 

        // Scan again
      }
      // Archetype
      else if (dynamic_cast<Archetype*>(resource)) {

      }



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
      DCTrace << "Content::ScanForLevels - Scanning for levels on the current project at: " << levelPath << "\n";
      //std::string LevelPath("Projects/Rebound/Resources/Levels/");

      // Load sound files
      std::vector<std::string> levels;
      if (!FileSystem::DirectoryListFilePaths(levelPath, levels))
        throw DCException("Content::ScanForLevels - Failed to load level files !");
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