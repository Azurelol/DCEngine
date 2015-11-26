/******************************************************************************/
/*!
@file   Content.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/23/2015
@brief
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "Content.h" 
#include "../Filesystem/FileSystem.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief Constructor for the Content system.
    */
    /**************************************************************************/
    Content::Content(std::string& coreAssetsPath) : 
                     System(std::string("ContentSystem"), EnumeratedSystem::Content),
                     CoreAssetsPath(coreAssetsPath) {
      ProjectInfo.reset(new ProjectData());
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Content system.
    @note  
    */
    /**************************************************************************/
    void Content::Initialize() {
      if (TRACE_INITIALIZE)
        DCTrace << "Content::Initialize \n";

      // Load the default resources of the engine's
      LoadCoreAssets();
    }

    /**************************************************************************/
    /*!
    @brief Loads the resources in every map.
    @note  Some resources need to be loaded after the systems they are used for
           have already been initialized.
    */
    /**************************************************************************/
    void Content::LoadAllResources()
    {
      // Load every SoundCue's sound
      for (auto soundCue : SoundCueMap) {
        soundCue.second->Load();
      }

      // Load every SpriteSource's texture
      for (auto texture : SpriteSourceMap) {
        texture.second->LoadTexture();
      }

      // Load every Font
      for (auto font : FontMap) {
        font.second->Load();
      }

    }
    
    /**************************************************************************/
    /*!
    @brief Deserializes a ProjectData file for project data settings.
    */
    /**************************************************************************/
    void Content::LoadProjectData(std::string& projectData)
    {

      //auto test = DirectoryExtractFilePaths(std::string("Core/Assets"));

      // Load the loaded project's assets
      LoadProjectAssets();

      DCTrace << "Content::LoadProjectData - Finished loading all project data. \n";
    }
    
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
      std::vector<std::string> coreSprites;
      if (!FileSystem::DirectoryListFilePaths(SpritePath, coreSprites))
        throw DCException("Content::LoadCoreAssets - Failed to load sprite files!");
      for (auto sprite : coreSprites) {
        auto spriteName = FileSystem::FileNoExtension (sprite);
        AddSpriteSource(spriteName, SpriteSourcePtr(new SpriteSource(sprite)));
      }
      // Load sound files
      std::vector<std::string> coreSounds;
      if (!FileSystem::DirectoryListFilePaths(SoundPath, coreSounds))
        throw DCException("Content::LoadCoreAssets - Failed to load sound files!");
      for (auto sound : coreSounds) {
        auto soundName = FileSystem::FileNoExtension(sound);
        AddSoundCue(soundName, SoundCuePtr(new SoundCue(sound)));
      }

      // Load default fonts      
      AddFont(std::string("Verdana"), FontPtr(new Font(FontPath + "Verdana.ttf")));
      DCTrace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";

      // Load archetypes
      std::vector<std::string> coreArchetypes;
      if (!FileSystem::DirectoryListFilePaths(ArchetypePath, coreArchetypes))
        throw DCException("Content::LoadCoreAssets - Failed to load archetype files!");
      for (auto archetype : coreArchetypes) {
        auto archetypeName = FileSystem::FileNoExtension(archetype);
        AddArchetype(archetypeName, ArchetypePtr(new Archetype(archetype)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the Level Path for level files.
    @param  A string containing the path of the project data file.
    @todo   Currently hardcoded the level path.
    */
    /**************************************************************************/
    void Content::ScanForLevels()
    {
      DCTrace << "Content::ScanForLevels - Scanning for levels on the current project \n";
      std::string LevelPath("Projects/Rebound/Resources/Levels/");

      // Load sound files
      std::vector<std::string> levels;
      if (!FileSystem::DirectoryListFilePaths(LevelPath, levels))
        throw DCException("Content::ScanForLevels - Failed to load level files!");
      for (auto level : levels) {
        auto soundName = FileSystem::FileNoExtension(level);
        AddLevel(soundName, LevelPtr(new Level(level)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Loads a project from file.
    @param  A string containing the path of the project data file.
    */
    /**************************************************************************/
    void Content::LoadProject(std::string projectDataPath)
    {
      std::string projectDataString;
      if (FileSystem::FileReadToString(projectDataPath, projectDataString))
        Serialization::Deserialize(ProjectInfo.get(), projectDataString);
    }

    /**************************************************************************/
    /*!
    @brief  Load all of the project's assets.
    */
    /**************************************************************************/
    void Content::LoadProjectAssets()
    {
      auto LevelPath = CoreAssetsPath + "Levels/";
      // Load levels
      std::vector<std::string> levels;
      if (!FileSystem::DirectoryListFilePaths(LevelPath, levels))
        throw DCException("Content::LoadCoreAssets - Failed to load archetype files!");
      for (auto level : levels) {
        auto archetypeName = FileSystem::FileNoExtension(level);
        AddLevel(archetypeName, LevelPtr(new Level(level)));
      }
    }

    /**************************************************************************/
    /*!
    @brief Grabs a shader resource.
    @return Returns a pointer to the shader object.
    */
    /**************************************************************************/
    ShaderPtr Content::getShader(std::string shaderName)
    {
      return ShaderMap.at(shaderName); 
    }

    /**************************************************************************/
    /*!
    @brief Grabs a font resource.
    @return Returns a pointer to the font object.
    */
    /**************************************************************************/
    FontPtr Content::getFont(std::string & fontName)
    {
      return FontMap.at(fontName);
    }

    /**************************************************************************/
    /*!
    @brief Grabs a SpriteSource resource.
    @return Returns a pointer to the spritesource object.
    */
    /**************************************************************************/
    SpriteSourcePtr Content::getSpriteSrc(std::string & spriteName)
    {
      // Check if the resource is present in the map
      if (!SpriteSourceMap.count(spriteName)) {
        //DCTrace << "Content::getSpriteSrc - " << spriteName << " was not found! Using default: \n";
        // Return a default '404 image not found.
        return SpriteSourceMap.at(DefaultImage);
      }

      //SpriteSourcePtr spriteSource(SpriteSourceMap.at(spriteName));
      //if (spriteSource == nullptr)
      //  throw DCException("Content::GetSpriteSource -" + spriteName + " was not found!");
      return SpriteSourceMap.at(spriteName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a SoundCue resource.
    @return Returns a pointer to the SoundCue object.
    */
    /**************************************************************************/
    SoundCuePtr Content::getSoundCue(std::string & soundCueName)
    {
      return SoundCueMap.at(soundCueName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs an Archetype resource.
    @return Returns a pointer to the Archetype object.
    */
    /**************************************************************************/
    ArchetypePtr Content::getArchetype(std::string & archetypeName)
    {
      return ArchetypeMap.at(archetypeName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a Level resource.
    @return Returns a pointer to the Level object.
    */
    /**************************************************************************/
    LevelPtr Content::getLevel(std::string & levelName)
    {
      // Check if the resource is present in the map
      if (!LevelMap.count(levelName)) {
        DCTrace << "Content::getLevel - " << levelName << " was not found!\n";
        return nullptr;
      }
      // If it does, first load it
      LevelMap.at(levelName)->Load();
      // Then return it
      return LevelMap.at(levelName);
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a CollisionGroup resource.
    @return Returns a pointer to the CollisionGroup object.
    */
    /**************************************************************************/
    CollisionGroupPtr Content::getCollisionGroup(std::string & groupName)
    {
      return CollisionGroupPtr();
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a CollisionTable resource.
    @return Returns a pointer to the CollisionTable object.
    */
    /**************************************************************************/
    CollisionTablePtr Content::getCollisionTable(std::string & groupName)
    {
      return CollisionTablePtr();
    }

    /**************************************************************************/
    /*!
    @brief Returns pointers to the content maps.
    @return Returns a pointer to the SoundCue object.
    */
    /**************************************************************************/
    SpriteSourceMap * Content::AllSpriteSources()
    {
      return &SpriteSourceMap;
    }

    SoundCueMap * Content::AllSoundCues()
    {
      return &SoundCueMap;
    }

    ShaderMap * Content::AllShaders()
    {
      return &ShaderMap;
    }

    ArchetypeMap * Content::AllArchetypes()
    {
      return &ArchetypeMap;
    }

    LevelMap * Content::AllLevels()
    {
      return &LevelMap;
    }




    /**************************************************************************/
    /*!
    @brief Updates the Content system.
    @note 
    */
    /**************************************************************************/
    void Content::Update(float dt) {
      // On every update, scan for resources being changed outside the application. (Hot loading?)
    }

    /**************************************************************************/
    /*!
    @brief Terminates the Content system.
    @note
    */
    /**************************************************************************/
    void Content::Terminate() {
      DCTrace << "Content::Terminate\n";
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
    @note  We load the archetype immediately so its ready for use.
    */
    /**************************************************************************/
    void Content::AddArchetype(std::string & archetypeName, ArchetypePtr archetypePtr)
    {
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
        DCTrace << "Content::AddLevel - " << levelName << " is already present in the map.\n";
        return;
      }
      
      LevelMap.insert(std::pair<std::string, LevelPtr>(levelName, levelPtr));
      DCTrace << "Content::AddLevel - " << levelName << " was added.\n";
    }

  }
}
