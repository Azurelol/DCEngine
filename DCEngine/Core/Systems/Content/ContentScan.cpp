/*****************************************************************************/
/*!
@file   ContentScan.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/6/2016
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Content.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Scans the project's resource path for updated levels.
    */
    /**************************************************************************/
    void Content::ScanForLevels()
    {
      ScanForLevels(ProjectInfo->ProjectPath + ProjectInfo->ResourcePath);
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

      // 2. Add the resource filepaths into a common pool
      ResourcePool pool;
      for (auto resourcePath : resources) {
        auto extension = FileSystem::FileExtension(resourcePath);
        pool[extension].push_back(resourcePath);
      }

      // Resources listed by extensions:
      //-----------------------------------------------------------//
      // Thread #1
      auto& spriteSources = pool[SpriteSource::Extension()];
      // Thread #2
      auto& soundCues = pool[SoundCue::Extension()];
      auto& banks = pool[Bank::Extension()];
      // Thread #3
      auto& fonts = pool[Font::Extension()];
      auto& spriteLayers = pool[SpriteLayer::Extension()];
      auto& spriteLayerOrders = pool[SpriteLayerOrder::Extension()];
      auto& levels = pool[Level::Extension()];
      auto& archetypes = pool[Archetype::Extension()];
      auto& collisionGroups = pool[CollisionGroup::Extension()];
      auto& collisionTables = pool[CollisionTable::Extension()];
      auto& physicsMaterials = pool[PhysicsMaterial::Extension()];
      auto& zilchScripts = pool[ZilchScript::Extension()];
      //-----------------------------------------------------------//
      // Add methods
      auto spriteSourceMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddSpriteSource), this, std::placeholders::_1);
      auto soundCueMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddSoundCue), this, std::placeholders::_1);
      auto fontMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddFont), this, std::placeholders::_1);
      auto bankMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddBank), this, std::placeholders::_1);
      auto spriteLayersMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddSpriteLayer), this, std::placeholders::_1);
      auto spriteLayerOrdersMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddSpriteLayerOrder), this, std::placeholders::_1);
      auto levelMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddLevel), this, std::placeholders::_1);
      auto archetypeMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddArchetype), this, std::placeholders::_1);
      auto collisionGroupsMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddCollisionGroup), this, std::placeholders::_1);
      auto collisionTablesMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddCollisionTable), this, std::placeholders::_1);
      auto physicsMaterialsMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddPhysicsMaterial), this, std::placeholders::_1);
      auto zilchScriptsMethod = std::bind(static_cast<void(Content::*)(const std::string&)>(&Content::AddZilchScript), this, std::placeholders::_1);
      //-------------------------------------------------------------//
      

      // Load SpriteSources using several threads.
      //auto threadSpriteSource = std::thread(AssetLoader::LoadS, spriteSources, spriteSourceMethod);
      //auto spriteSourcesBySize
      // Sort the spritesource files by size

      //AssetLoader loader;
      //loader.Load(spriteSources, spriteSourceMethod);
      


      // 3. Add the resources into the content system by each extension,
      //    by different threads
      // @todo: This may crash if they are not there?
      //for (auto& extension : pool) {
      //  for (auto& spriteSource : pool[SpriteSource::Extension()]) {
      //    AddSpriteSource(spriteSource);
      //  }
      //
      //}

      // 2. Parse the resources and add them to their appropiate maps
      for (auto resource : resources) {
        // 1. Get the resource's name and extension
        auto resourceName = FileSystem::FileNoExtension(resource);
        auto extension = FileSystem::FileExtension(resource);
        // 2. Depending on the extension, add the specific resource:
        if (extension == SpriteSource::Extension())
          AddSpriteSource(resource);
        if (extension == SpriteLayer::Extension())
          AddSpriteLayer(resource);
        else if (extension == SpriteLayerOrder::Extension())
          AddSpriteLayerOrder(resource);
        else if (extension == Font::Extension())
          AddFont(resource);
        else if (extension == SoundCue::Extension())
          AddSoundCue(resource);
        else if (extension == Level::Extension())
          AddLevel(resource);
        else if (extension == Archetype::Extension())
          AddArchetype(resource);
        else if (extension == CollisionGroup::Extension())
          AddCollisionGroup(resource);
        else if (extension == CollisionTable::Extension())
          AddCollisionTable(resource);
        else if (extension == PhysicsMaterial::Extension())
          AddPhysicsMaterial(resource);
        else if (extension == ZilchScript::Extension())
          AddZilchScript(resource);
        else if (extension == Bank::Extension())
          AddBank(resource);
      }

      // Wait until all threads are finished before leaving scope
      //if (threadSpriteSource.joinable()) threadSpriteSource.join();


    }

    /**************************************************************************/
    /*!
    @brief  Scans the engine's asset path for resources to generate and add
    to the engine.
    @todo   Make it scan recursively.
    */
    /**************************************************************************/
    void Content::ScanAndGenerateResources()
    {
      //auto SpritePath = CoreAssetsPath + "Sprites/";
      //auto SoundPath = CoreAssetsPath + "Sounds/";
      //auto FontPath = CoreAssetsPath + "Fonts/";
      //// 1. Scan for all resources in the specified directory
      //std::vector<std::string> resources;
      //if (!FileSystem::DirectoryListFilePaths(resourcePath, resources)) {
      //  auto exceptionInfo = std::string("Content::ScanResources - Failed to find the resource directory: " + ProjectInfo->ResourcePath);
      //  throw DCException(exceptionInfo);
      //}

      //// 2. Parse the resources and add them to their appropiate maps
      //for (auto resource : resources) {
      //  // 1. Get the resource's name and extension
      //  auto resourceName = FileSystem::FileNoExtension(resource);
      //  auto extension = FileSystem::FileExtension(resource);
      //  // 2. Depending on the extension, add the specific resource:
      //  if (extension == SpriteSource::Extension())
      //    AddSpriteSource(resourceName, SpriteSourcePtr(new SpriteSource(resource)));
      //}
    }



    void Content::ScanForArchetypes()
    {
      ScanForArchetypes(ProjectInfo->ProjectPath + ProjectInfo->ResourcePath);
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
    void Content::ScanForLevels(std::string levelPath)
    {
      DCTrace << "Content::ScanForLevels - Scanning for levels on the current project at: " << levelPath << "\n";
      //std::string LevelPath("Projects/Rebound/Resources/Levels/");

      std::vector<std::string> levels;
      if (!FileSystem::DirectoryListFilePaths(levelPath, levels, Level::Extension())) {
        auto exceptionInfo = std::string("Content::ScanForLevels - Failed to find level directory: " + levelPath);
        throw DCException(exceptionInfo);
      }
      for (auto level : levels) {
        auto name = FileSystem::FileNoExtension(level);
        AddLevel(name, LevelPtr(new Level(level)));
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the specified path for archetype files.
    */
    /**************************************************************************/
    void Content::ScanForArchetypes(std::string archetypePath)
    {
      DCTrace << "Content::ScanForArchetypes - Scanning for archetypes on the current project \n";
      //std::string ArchetypePath("Projects/Rebound/Resources/Archetypes/");

      // Load sound files
      std::vector<std::string> archetypes;
      if (!FileSystem::DirectoryListFilePaths(archetypePath, archetypes, Archetype::Extension())) {
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
    void Content::ScanForSpriteSources(std::string  spriteSourcePath)
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
    void Content::ScanForSoundCues(std::string soundCuePath)
    {
      // Load sound files
      std::vector<std::string> soundCues;
      if (!FileSystem::DirectoryListFilePaths(soundCuePath, soundCues)) {
        auto exceptionInfo = std::string("Content::ScanForSoundCues - Failed to find sound directory: " + soundCuePath);
        throw DCException(exceptionInfo);
      }
      for (auto sound : soundCues) {
        auto soundName = FileSystem::FileNoExtension(sound);
        auto soundCue = SoundCuePtr(new SoundCue(sound, SoundCue::SoundCueType::File));
        soundCue->setAssetPath(sound);
        AddSoundCue(soundName, soundCue);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Scans the specified path for Font files.
    */
    /**************************************************************************/
    void Content::ScanForFonts(std::string fontPath)
    {
      DCTrace << "Content::ScanForFonts - Scanning the current project \n";

      // Load sprites
      std::vector<std::string> fonts;
      if (!FileSystem::DirectoryListFilePaths(fontPath, fonts)) {
        auto exceptionInfo = std::string("Content::ScanForFonts - Failed to find font directory: " + fontPath);
        throw DCException(exceptionInfo);
      }

      for (auto font : fonts) {
        auto fontName = FileSystem::FileNoExtension(font);
        auto fontResource = FontPtr(new Font(font));
        fontResource->setAssetPath(font);
        AddFont(fontName, fontResource);
        fontResource->LoadFontFromFile();
        fontResource->GenerateFont();
      }
    }

  }
}