/*****************************************************************************/
/*!
@file   ContentAdd.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
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
      //AddFont(std::string("Verdana"), FontPtr(new Font(FontPath + "Verdana.ttf")));
      //DCTrace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";
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
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
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
        if (DCE_TRACE_FACTORY_RESOURCE_ADD)
          DCTrace << "Content::AddArchetype - " << archetypeName << " is already present in the map. Overwriting \n";
        //return;
      }

      ArchetypeMap.insert(std::pair<std::string, ArchetypePtr>(archetypeName, archetypePtr));
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
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
        if (DCE_TRACE_FACTORY_RESOURCE_ADD)
          DCTrace << "Content::AddSpriteSource - " << spriteSourceName << " is already present in the map.\n";
        return;
      }

      SpriteSourceMap.insert(std::pair<std::string, SpriteSourcePtr>(spriteSourceName, spriteSourcePtr));
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
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
        if (DCE_TRACE_FACTORY_RESOURCE_ADD)
          DCTrace << "Content::AddSoundCue - " << soundCueName << " is already present in the map.\n";
        return;
      }

      SoundCueMap.insert(std::pair<std::string, SoundCuePtr>(soundCueName, soundcuePtr));
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
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
        if (DCE_TRACE_FACTORY_RESOURCE_ADD)
          DCTrace << "Content::AddLevel - " << levelName << " is already present in the map. Overwriting. \n";
        //return;
      }

      LevelMap.insert(std::pair<std::string, LevelPtr>(levelName, levelPtr));
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
        DCTrace << "Content::AddLevel - " << levelName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a CollisionGroup resource to the engine.
    @param The name of the CollisionGroup.
    @param The pointer to the CollisionGroup resource.
    */
    /**************************************************************************/
    void Content::AddCollisionGroup(std::string & collisionGroupName, CollisionGroupPtr collisionGroupPtr)
    {
      AddResourceToMap<CollisionGroupPtr, CollisionGroupMap>(collisionGroupName, collisionGroupPtr, MapCollisionGroup);
    }

    /**************************************************************************/
    /*!
    @brief Adds a CollisionTable resource to the engine.
    @param The name of the CollisionTable.
    @param The pointer to the CollisionTable resource.
    */
    /**************************************************************************/
    void Content::AddCollisionTable(std::string & collisionTableName, CollisionTablePtr collisionTablePtr)
    {
      AddResourceToMap<CollisionTablePtr, CollisionTableMap>(collisionTableName, collisionTablePtr, MapCollisionTable);
    }

    /**************************************************************************/
    /*!
    @brief Adds a ZilchScript resource to the engine.
    @param The name of the ZilchScript.
    @param The pointer to the ZilchScript resource.
    */
    /**************************************************************************/
    void Content::AddZilchScript(std::string & zilchScriptName, ZilchScriptPtr zilchScriptPtr)
    {
      AddResourceToMap<ZilchScriptPtr, ZilchScriptMap>(zilchScriptName, zilchScriptPtr, MapZilchScript);
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

    /**************************************************************************/
    /*!
    @brief  Scans the project's resource path for resources to add to the engine.
    */
    /**************************************************************************/
    void Content::ScanResources()
    {
      auto resourcePath = ProjectInfo->ProjectPath + ProjectInfo->ResourcePath;    
      // 1. Scan for all resources in the specified directory
      std::vector<std::string> resources;
      if (!FileSystem::DirectoryListFilePaths(resourcePath, resources)) {
        auto exceptionInfo = std::string("Content::ScanResources - Failed to find the resource directory: " + ProjectInfo->ResourcePath);
        throw DCException(exceptionInfo);
      }

      // 2. Parse the resources and add them to their appropiate maps
      for (auto resource : resources) {
        // 1. Get the resource's name and extension
        auto resourceName = FileSystem::FileNoExtension(resource);
        auto extension = FileSystem::FileExtension(resource);
        // 2. Depending on the extension, add the specific resource:
        if (extension == SpriteSource::Extension())
          AddSpriteSource(resourceName, SpriteSourcePtr(new SpriteSource(resource)));
        else if (extension == SoundCue::Extension())
          AddSoundCue(resourceName, SoundCuePtr(new SoundCue(resource)));
        else if (extension == Level::Extension())
          AddLevel(resourceName, LevelPtr(new Level(resource)));        
        else if (extension == Archetype::Extension())
          AddArchetype(resourceName, ArchetypePtr(new Archetype(resource)));
        else if (extension == CollisionGroup::Extension())
          AddCollisionGroup(resourceName, CollisionGroupPtr(new CollisionGroup(resource)));
        else if (extension == CollisionTable::Extension())
          AddCollisionTable(resourceName, CollisionTablePtr(new CollisionTable(resource)));
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
      if (!FileSystem::DirectoryListFilePaths(levelPath, levels)) {
        auto exceptionInfo = std::string("Content::ScanForLevels - Failed to find level directory: " + levelPath);
        throw DCException(exceptionInfo);
      }
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
      if (!FileSystem::DirectoryListFilePaths(archetypePath, archetypes)) {
        auto exceptionInfo = std::string("Content::ScanForArchetypes - Failed to find archetype directory: " + archetypePath);
        throw DCException(exceptionInfo);      
      }
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
      DCTrace << "Content::ScanForSpriteSources - Scanning for spritesources on the current project \n";

      // Load sprites
      std::vector<std::string> sprites;
      if (!FileSystem::DirectoryListFilePaths(spriteSourcePath, sprites)) {
        auto exceptionInfo = std::string("Content::ScanForSpriteSources - Failed to find sprite directory: " + spriteSourcePath);
        throw DCException(exceptionInfo);
      }
        
      for (auto sprite : sprites) {
        auto spriteName = FileSystem::FileNoExtension(sprite);
        auto spriteSource = SpriteSourcePtr(new SpriteSource(sprite));
        spriteSource->setAssetPath(sprite);
        AddSpriteSource(spriteName, spriteSource);
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
      if (!FileSystem::DirectoryListFilePaths(soundCuePath, soundCues)) {
        auto exceptionInfo = std::string("Content::ScanForSoundCues - Failed to find sound directory: " + soundCuePath);
        throw DCException(exceptionInfo);
      }
      for (auto sound : soundCues) {
        auto soundName = FileSystem::FileNoExtension(sound);
        auto soundCue = SoundCuePtr(new SoundCue(sound));
        soundCue->setAssetPath(sound);
        AddSoundCue(soundName, soundCue);
      }
    }

    ProjectDataPtr & Content::ProjectSettings()
    {
      return ProjectInfo;
    }

  }
}