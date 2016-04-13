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
#include "../../Engine/Engine.h"

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
    void Content::LoadCoreResources()
    {
      DCTrace << "\n[Content::LoadDefaultResources] - Loading default resources \n";

      // In the future, find a way to dynamically find the folder paths?
      auto SpritePath = Settings.DefaultAssetPath + "Sprites/";
      auto SoundPath = Settings.DefaultAssetPath + "Sounds/";
      auto FontPath = Settings.DefaultAssetPath + "Fonts/";
      auto ShaderPath = Settings.DefaultAssetPath + "Shaders/";
      auto ArchetypePath = Settings.DefaultAssetPath + "Archetypes/";
      auto LevelsPath = Settings.DefaultAssetPath + "Levels/";

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
      AddShader(std::string("ParticleShader"), ShaderPtr(new Shader(std::string("ParticleShader"),
        ShaderPath + "ParticleShader.vs",
        ShaderPath + "ParticleShader.frag")));
      AddShader(std::string("ShadowingShader"), ShaderPtr(new Shader(std::string("ShadowingShader"),
        ShaderPath + "ShadowingShader.vs",
        ShaderPath + "ShadowingShader.frag",
        ShaderPath + "ShadowingShader.gs")));
      AddShader(std::string("LightingShader"), ShaderPtr(new Shader(std::string("LightingShader"),
        ShaderPath + "LightingShader.vs",
        ShaderPath + "LightingShader.frag")));
      AddShader(std::string("DebugShader"), ShaderPtr(new Shader(std::string("DebugShader"),
        ShaderPath + "DebugShader.vs",
        ShaderPath + "DebugShader.frag")));
      AddShader(std::string("FinalRenderShader"), ShaderPtr(new Shader(std::string("FinalRenderShader"),
        ShaderPath + "FinalRenderShader.vs",
        ShaderPath + "FinalRenderShader.frag")));
      
      // Add the loading screen level
      //AddLevel("LoadingScreen", LevelPtr(new ))

      // Scan for default assets and generate resources
      ScanForLevels(LevelsPath);
      // Load sprites
      ScanForSpriteSources(SpritePath);
      // Load sound files
      ScanForSoundCues(SoundPath);
      // Load default fonts      
      ScanForFonts(FontPath);
      // Load archetypes
      ScanForArchetypes(ArchetypePath);
      // Generates default resources
      GenerateDefaultResources();
      //DCTrace << "[Content::LoadDefaultResources] - Finished loading default resources \n\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a resource to the map.
    */
    /**************************************************************************/
    void Content::AddFont(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = FontPtr(new Font(resourcePath));
      AddResourceToMap<FontPtr, FontMap>(resourceName, resourcePtr, MapFont);
    }

    void Content::AddArchetype(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = ArchetypePtr(new Archetype(resourcePath));
      AddResourceToMap<ArchetypePtr, ArchetypeMap>(resourceName, resourcePtr, MapArchetype);
    }

    void Content::AddShader(const std::string & resourcePath)
    {
      //auto resourceName = FileSystem::FileNoExtension(resourcePath);
      //auto resourcePtr = ShaderPtr(new Shader(resourcePath));
      //AddResourceToMap<ShaderPtr, ShaderMap>(resourceName, resourcePtr, MapShader);
    }

    void Content::AddSpriteSource(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = SpriteSourcePtr(new SpriteSource(resourcePath));
      AddResourceToMap<SpriteSourcePtr, SpriteSourceMap>(resourceName, resourcePtr, MapSpriteSource);
    }

    void Content::AddSoundCue(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = SoundCuePtr(new SoundCue(resourcePath, SoundCue::SoundCueType::File));
      AddResourceToMap<SoundCuePtr, SoundCueMap>(resourceName, resourcePtr, MapSoundCue);
    }

    void Content::AddBank(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = BankPtr(new Bank(resourcePath));
      AddResourceToMap<BankPtr, BankMap>(resourceName, resourcePtr, MapBank);
    }

    void Content::AddLevel(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = LevelPtr(new Level(resourcePath));
      AddResourceToMap<LevelPtr, LevelMap>(resourceName, resourcePtr, MapLevel);
    }

    void Content::AddCollisionGroup(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = CollisionGroupPtr(new CollisionGroup(resourcePath));
      AddResourceToMap<CollisionGroupPtr, CollisionGroupMap>(resourceName, resourcePtr, MapCollisionGroup);
    }

    void Content::AddCollisionTable(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = CollisionTablePtr(new CollisionTable (resourcePath));
      AddResourceToMap<CollisionTablePtr, CollisionTableMap>(resourceName, resourcePtr, MapCollisionTable);
    }

    void Content::AddPhysicsMaterial(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = PhysicsMaterialPtr(new PhysicsMaterial(resourcePath));
      AddResourceToMap<PhysicsMaterialPtr, PhysicsMaterialMap>(resourceName, resourcePtr, MapPhysicsMaterial);
    }

    void Content::AddZilchScript(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = ZilchScriptPtr(new ZilchScript(resourcePath));
      AddResourceToMap<ZilchScriptPtr, ZilchScriptMap>(resourceName, resourcePtr, MapZilchScript);
    }

    void Content::AddSpriteLayer(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = SpriteLayerPtr(new SpriteLayer(resourcePath));
      AddResourceToMap<SpriteLayerPtr, SpriteLayerMap>(resourceName, resourcePtr, MapSpriteLayer);
    }

    void Content::AddSpriteLayerOrder(const std::string & resourcePath)
    {
      auto resourceName = FileSystem::FileNoExtension(resourcePath);
      auto resourcePtr = SpriteLayerOrderPtr(new SpriteLayerOrder(resourcePath));
      AddResourceToMap<SpriteLayerOrderPtr, SpriteLayerOrderMap>(resourceName, resourcePtr, MapSpriteLayerOrder);
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
    void Content::AddFont(const std::string & fontName, FontPtr fontPtr)
    {
      MapFont.insert(std::pair<std::string, FontPtr>(fontName, fontPtr));
      if (Debug::TraceFactoryResourceAdd)
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
    void Content::AddArchetype(const std::string & archetypeName, ArchetypePtr archetypePtr)
    {


      // Overwrite archetypes
      if (MapArchetype.count(archetypeName)) {
        MapArchetype.erase(archetypeName);
        if (Debug::TraceFactoryResourceAdd)
          DCTrace << "Content::AddArchetype - " << archetypeName << " is already present in the map. Overwriting \n";
        //return;
      }

      MapArchetype.insert(std::pair<const std::string, ArchetypePtr>(archetypeName, archetypePtr));
      if (Debug::TraceFactoryResourceAdd)
        DCTrace << "Content::AddArchetype - " << archetypeName << " was added.\n";              
    }

    /**************************************************************************/
    /*!
    @brief Adds a shader to the shader resource map.
    @param The name of the shader.
    @param The pointer to the shader resource.
    */
    /**************************************************************************/
    void Content::AddShader(const std::string & shaderName, ShaderPtr shaderPtr)
    {
      MapShader.insert(std::pair<std::string, ShaderPtr>(shaderName, shaderPtr));
      DCTrace << "Content::AddShader - " << shaderName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a spritesource to the spritesource resource map.
    @param The name of the spritesource.
    @param The pointer to the spritesource resource.
    */
    /**************************************************************************/
    void Content::AddSpriteSource(const std::string & spriteSourceName, SpriteSourcePtr spriteSourcePtr)
    {
      // Prevent duplicates
      if (MapSpriteSource.count(spriteSourceName)) {
        if (Debug::TraceFactoryResourceAdd)
          DCTrace << "Content::AddSpriteSource - " << spriteSourceName << " is already present in the map.\n";
        return;
      }

      MapSpriteSource.insert(std::pair<std::string, SpriteSourcePtr>(spriteSourceName, spriteSourcePtr));
      if (Debug::TraceFactoryResourceAdd)
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
      if (MapSoundCue.count(soundCueName)) {
        if (Debug::TraceFactoryResourceAdd)
          DCTrace << "Content::AddSoundCue - " << soundCueName << " is already present in the map.\n";
        return;
      }

      MapSoundCue.insert(std::pair<std::string, SoundCuePtr>(soundCueName, soundcuePtr));
      if (Debug::TraceFactoryResourceAdd)
        DCTrace << "Content::AddSoundCue - " << soundCueName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a bank resource to the Bank resource map.
    @param The name of the bank.
    @param The pointer to the bank resource.
    */
    /**************************************************************************/
    void Content::AddBank(const std::string & bankName, BankPtr bankPtr)
    {
      AddResourceToMap<BankPtr, BankMap>(bankName, bankPtr, MapBank);
    }

    /**************************************************************************/
    /*!
    @brief Adds a level resource to the Level resource map.
    @param The name of the level.
    @param The pointer to the level resource.
    */
    /**************************************************************************/
    void Content::AddLevel(const std::string & levelName, LevelPtr levelPtr)
    {
      if (MapLevel.count(levelName)) {
        // Overwrite the current level
        MapLevel.erase(levelName);
        if (Debug::TraceFactoryResourceAdd)
          DCTrace << "Content::AddLevel - " << levelName << " is already present in the map. Overwriting. \n";
        //return;
      }

      MapLevel.insert(std::pair<std::string, LevelPtr>(levelName, levelPtr));
      if (Debug::TraceFactoryResourceAdd)
        DCTrace << "Content::AddLevel - " << levelName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief Adds a CollisionGroup resource to the engine.
    @param The name of the CollisionGroup.
    @param The pointer to the CollisionGroup resource.
    */
    /**************************************************************************/
    void Content::AddCollisionGroup(const std::string & collisionGroupName, CollisionGroupPtr collisionGroupPtr)
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
    void Content::AddCollisionTable(const std::string & collisionTableName, CollisionTablePtr collisionTablePtr)
    {
      AddResourceToMap<CollisionTablePtr, CollisionTableMap>(collisionTableName, collisionTablePtr, MapCollisionTable);
    }

    void Content::AddPhysicsMaterial(const std::string & physicsMaterialName, PhysicsMaterialPtr physicsMaterialPtr)
    {
      AddResourceToMap<PhysicsMaterialPtr, PhysicsMaterialMap>(physicsMaterialName, physicsMaterialPtr, MapPhysicsMaterial);
    }

    /**************************************************************************/
    /*!
    @brief Adds a ZilchScript resource to the engine.
    @param The name of the ZilchScript.
    @param The pointer to the ZilchScript resource.
    */
    /**************************************************************************/
    void Content::AddAndLoadZilchScript(const  std::string & zilchScriptName, ZilchScriptPtr zilchScriptPtr)
    {
      AddResourceToMap<ZilchScriptPtr, ZilchScriptMap>(zilchScriptName, zilchScriptPtr, MapZilchScript);
      // Load the script
      zilchScriptPtr->Load();
      // Add the the script to the scripting systems's library
      zilchScriptPtr->IncludeScript();
      // Recompile the scripts library again so it can be used immediately.
      Daisy->getSystem<Reflection>()->Handler()->CompileScripts();
    }

    void Content::AddSpriteLayer(const std::string & name, SpriteLayerPtr ptr)
    {
      AddResourceToMap<SpriteLayerPtr, SpriteLayerMap>(name, ptr, MapSpriteLayer);
    }

    void Content::AddSpriteLayerOrder(const std::string & name, SpriteLayerOrderPtr ptr)
    {
      AddResourceToMap<SpriteLayerOrderPtr, SpriteLayerOrderMap>(name, ptr, MapSpriteLayerOrder);
    }

    
    /**************************************************************************/
    /*!
    @brief Generates default resources to be used by the engine.
    */
    /**************************************************************************/
    void Content::GenerateDefaultResources()
    {
      AddSpriteLayer(Settings.DefaultSpriteLayer, SpriteLayerPtr(new SpriteLayer(Settings.DefaultSpriteLayer)));
      AddSpriteLayerOrder(Settings.DefaultSpriteLayerOrder, SpriteLayerOrderPtr(new SpriteLayerOrder(Settings.DefaultSpriteLayerOrder)));
      AddCollisionGroup( Settings.DefaultCollisionGroup, CollisionGroupPtr(new CollisionGroup(   Settings.DefaultCollisionGroup)));
      AddCollisionTable( Settings.DefaultCollisionTable, CollisionTablePtr(new CollisionTable(   Settings.DefaultCollisionTable)));
      AddPhysicsMaterial(Settings.DefaultPhysicsMaterial, PhysicsMaterialPtr(new PhysicsMaterial(Settings.DefaultPhysicsMaterial)));
    }

    /**************************************************************************/
    /*!
    @brief Returns a reference to the ProjectSettings data struct.
    */
    /**************************************************************************/
    ProjectDataPtr & Content::ProjectSettings()
    {
      return ProjectInfo;
    }

  }
}