/*****************************************************************************/
/*!
\file   Content.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/20/2015
\brief  The Content system handles the loading of all content data required
        for a project. It stores references to the names of these resources
        as well as their file locations. The deserialization of these files
        is handled by the factory system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Resources/ResourcesInclude.h"
#include "../../Engine/Data.h"

namespace DCEngine {

  class Engine;
  class Factory;

  namespace Systems {

    // Resource Maps
    using ResourceMap = std::map<std::string, std::string>;    
    using SpriteSourceMap = std::map<std::string, SpriteSourcePtr>;
    using SoundCueMap = std::map<std::string, SoundCuePtr>;
    using ShaderMap = std::map<std::string, ShaderPtr>;
    using ArchetypeMap = std::map<std::string, ArchetypePtr>;
    using LevelMap = std::map<std::string, LevelPtr>;
    using CollisionTableMap = std::map<std::string, CollisionTablePtr>;
    using CollisionGroupMap = std::map<std::string, CollisionGroupPtr>;
    using PhysicsMaterialMap = std::map<std::string, PhysicsMaterialPtr>;    
    using ZilchScriptMap = std::map<std::string, ZilchScriptPtr>;
    
    class Content : public System {
      friend class Engine;
      friend class Factory;
      friend class Editor;
    public:    
      void LoadProjectResources(); //!< Load resources from a project.
      
      // Individual getters
      ShaderPtr getShader(std::string shaderName);
      FontPtr getFont(std::string& fontName);
      SpriteSourcePtr getSpriteSrc(std::string& spriteName);
      SoundCuePtr getSoundCue(std::string& soundCueName);
      ArchetypePtr getArchetype(std::string& archetypeName);
      LevelPtr getLevel(std::string& levelName);
      CollisionGroupPtr getCollisionGroup(std::string& groupName);
      CollisionTablePtr getCollisionTable(std::string& tableName);
      ZilchScriptPtr getZilchScript(std::string& scriptName);
      PhysicsMaterialPtr getPhysicsMaterial(std::string& materialName);
      template <typename ResourceMap, typename ResourcePtr>
      ResourcePtr getResource(std::string& resourceName, ResourceMap map);
      // Container getters
      SpriteSourceMap* AllSpriteSources();
      SoundCueMap* AllSoundCues();
      ShaderMap* AllShaders();
      ArchetypeMap* AllArchetypes();
      LevelMap* AllLevels();
      ZilchScriptMap* AllZilchScripts();
      CollisionGroupMap* AllCollisionGroups();
      CollisionTableMap* AllCollisionTables();
      PhysicsMaterialMap* AllPhysicsMaterials();
      // Remove resource
      void RemoveResource(ResourcePtr);      
      // Scanners
      void ScanResources();
      void ScanForLevels();
      void ScanForArchetypes();
      void ScanForSpriteSources();
      void ScanForSoundCues();
      void ScanForLevels(std::string& levelPath);
      void ScanForArchetypes(std::string& archetypePath);
      void ScanForSpriteSources(std::string& spriteSourcePath);
      void ScanForSoundCues(std::string& soundCuePath);
      // Get the current project's settings
      ProjectDataPtr& ProjectSettings();

    private:
      
      // Data
      std::string CoreAssetsPath;
      ProjectDataPtr ProjectInfo;
      std::string DefaultImage = "Wow";
      //std::string DefaultSound = "Eh";

      // Resource maps      
      std::map<std::string, ShaderPtr> ShaderMap;
      std::map<std::string, FontPtr> FontMap;
      SoundCueMap SoundCueMap;
      SpriteSourceMap SpriteSourceMap;      
      ArchetypeMap ArchetypeMap;
      LevelMap LevelMap;
      CollisionGroupMap MapCollisionGroup;
      CollisionTableMap MapCollisionTable;
      ZilchScriptMap MapZilchScript;
      PhysicsMaterialMap MapPhysicsMaterial;

      // Map functions
      void AddFont(std::string& fontName, FontPtr fontPtr);
      void AddArchetype(std::string& archetypeName, ArchetypePtr archetypePtr);
      void AddShader(std::string& shaderName, ShaderPtr shaderPtr);
      void AddSpriteSource(std::string& spriteSourceName, SpriteSourcePtr spriteSourcePtr);
      void AddSoundCue(std::string& soundCueName, SoundCuePtr soundcuePtr);      
      void AddLevel(std::string& levelName, LevelPtr levelPtr);
      void AddCollisionGroup(std::string& collisionGroupName, CollisionGroupPtr collisionGroupPtr);
      void AddCollisionTable(std::string& collisionTableName, CollisionTablePtr collisionTablePtr);
      void AddPhysicsMaterial(std::string& physicsMaterialName, PhysicsMaterialPtr physicsMaterialPtr);
      void AddZilchScript(std::string& zilchScriptName, ZilchScriptPtr zilchScriptPtr);
      template <typename ResourcePtr, typename ResourceMap>
      void AddResourceToMap(std::string& resourceName, ResourcePtr ptr, ResourceMap& map);

      // Core functions
      Content(std::string& coreAssetsPath);
      void Initialize();
      void Update(float dt);
      void Terminate();
      // Loading functions
      void LoadCoreAssets(); //!< Load default content files for the engine.   

      void LoadProject(std::string projectDataPath);
      void LoadProjectAssets(); //!< Load the assets used by the loaded project.      
      void LoadAllResources();

      void LoadProjectData(std::string&); //!<             
    };


    // Templates    
    /**************************************************************************/
    /*!
    @brief Adds a resource to its resource map in the engine.
    @param resourceName The name of the resource.
    @param ptr A pointer to the resource.
    @param map A reference to the map to use.
    */
    /**************************************************************************/
    template<typename ResourcePtr, typename ResourceMap>
    inline void Content::AddResourceToMap(std::string & resourceName, ResourcePtr ptr, ResourceMap& map)
    {
      // Prevent duplicates
      if (map.count(resourceName)) {
        // Overwrite the current level
        map.erase(resourceName);
        if (DCE_TRACE_FACTORY_RESOURCE_ADD)
          DCTrace << "Content::AddResourceToMap - " << resourceName << " is already present in the map. Overwriting. \n";
        //return;
      }

      map.insert(std::pair<std::string, ResourcePtr>(resourceName, ptr));
      if (DCE_TRACE_FACTORY_RESOURCE_ADD)
        DCTrace << "Content::AddResourceToMap - " << resourceName << " was added.\n";
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a resource from the specified resource map.
    @param  resourceName The name of the resource.
    @return A pointer of the resource type in question.
    */
    /**************************************************************************/
    template<typename ResourceMap, typename ResourcePtr>
    inline ResourcePtr Content::getResource(std::string & resourceName, ResourceMap map)
    {
      // Check if the resource is present in the map
      if (!map.count(resourceName)) {
        DCTrace << "Content::getResource - " << resourceName << " was not found!\n";
        return nullptr;
      }
      // If it does, first load it
      map.at(resourceName)->Load();
      // Then return it
      return LevelMap.at(levelName);
    }

  }

}

