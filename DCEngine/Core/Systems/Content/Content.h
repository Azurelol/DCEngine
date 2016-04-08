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

#include "FileScanner.h"
#include "AssetLoader.h"
#include "ContentUtils.h"
#include "../../Resources/ResourcesInclude.h"
#include "../../Engine/Data.h"

namespace DCEngine {

  class Engine;
  class Factory;
  
  namespace Systems {
    
    // Resource Maps
    using SpriteSourceMap = std::map<std::string, SpriteSourcePtr>;
    using SpriteLayerMap = std::map<std::string, SpriteLayerPtr>;
    using SpriteLayerOrderMap = std::map<std::string, SpriteLayerOrderPtr>;
    using SoundCueMap = std::map<std::string, SoundCuePtr>;
    using BankMap = std::map<std::string, BankPtr>;
    using ShaderMap = std::map<std::string, ShaderPtr>;
    using FontMap = std::map<std::string, FontPtr>;
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

      ResourceQueue& LoadedGraphicalResources() { return this->LoadedGraphicalResourcesQueue; }
      void LoadProjectResources(); //!< Load resources from a project.
      
      // Individual getters
      BankPtr getBank                           (const std::string bankName);
      ShaderPtr getShader                       (const std::string shaderName);
      FontPtr getFont                           (const std::string& fontName);
      SpriteSourcePtr getSpriteSrc              (const std::string& spriteName);
      SoundCuePtr getSoundCue                   (const std::string& soundCueName);
      ArchetypePtr getArchetype                 (const std::string& archetypeName);
      LevelPtr getLevel                         (const std::string& levelName);
      CollisionGroupPtr getCollisionGroup       (const std::string& groupName);
      CollisionTablePtr getCollisionTable       (const std::string& tableName);
      ZilchScriptPtr getZilchScript             (const std::string& scriptName);
      PhysicsMaterialPtr getPhysicsMaterial     (const std::string& materialName);
      SpriteLayerPtr getSpriteLayer             (const std::string& name);
      TexturePtr getTexture                     (const std::string& name);
      SpriteLayerOrderPtr getSpriteLayerOrder   (const std::string& name);
      template <typename ResourceMap, typename ResourcePtr>
      ResourcePtr getResource(const std::string& resourceName, ResourceMap map, std::string& defaultResource);
      // Container getters
      TextureMap* AllTextures();
      SpriteSourceMap* AllSpriteSources();
      SoundCueMap* AllSoundCues();
      BankMap* AllBanks();
      ShaderMap* AllShaders();
      FontMap* AllFonts();
      ArchetypeMap* AllArchetypes();
      LevelMap* AllLevels();
      ZilchScriptMap* AllZilchScripts();
      CollisionGroupMap* AllCollisionGroups();
      CollisionTableMap* AllCollisionTables();
      PhysicsMaterialMap* AllPhysicsMaterials();
      SpriteLayerMap* AllSpriteLayers();
      SpriteLayerOrderMap* AllSpriteLayerOrders();
      // Scanners
      void ScanResources();
      void ScanForLevels();
      void ScanForArchetypes();
      void ScanForSpriteSources();
      void ScanForSoundCues();
      void ScanForLevels(std::string levelPath);
      void ScanForArchetypes(std::string archetypePath);
      void ScanForSpriteSources(std::string spriteSourcePath);
      void ScanForSoundCues(std::string soundCuePath);
      void ScanForFonts(std::string fontPath);
      void ScanAndGenerateResources();
      // Get the current project's settings
      ProjectDataPtr& ProjectSettings();

      // Maybe.. add the others too.
      void AddSoundCue(std::string& soundCueName, SoundCuePtr soundcuePtr);            
      // Projects
      void LoadProject(const std::string& projectDataPath);
      void SaveProject(const std::string& projectPath);
      ProjectProperties& getProjectInfo() { return *(this->ProjectInfo.get()); }
      // DTOR
      ~Content();

    private:      
      ContentConfig& Settings;
      std::thread LoadingThread;
      bool Loading;
      // Data
      ProjectDataPtr ProjectInfo;
      std::unique_ptr<FileScanner> ProjectScanner;
      // Renderable
      ResourceQueue LoadedGraphicalResourcesQueue;

      // Resource maps      
      std::map<std::string, ShaderPtr> MapShader;
      FontMap MapFont;
      SoundCueMap MapSoundCue;
      SpriteSourceMap MapSpriteSource;
      ArchetypeMap MapArchetype;
      LevelMap MapLevel;
      CollisionGroupMap MapCollisionGroup;
      CollisionTableMap MapCollisionTable;
      ZilchScriptMap MapZilchScript;
      PhysicsMaterialMap MapPhysicsMaterial;
      SpriteLayerMap MapSpriteLayer;
      SpriteLayerOrderMap MapSpriteLayerOrder;
      BankMap MapBank;
      TextureMap MapTextures;
      // Newer, shinier add functions
      template <typename ResourcePtr, typename ResourceMap>
      void AddResourceToMap(const std::string& resourceName, ResourcePtr ptr, ResourceMap& map);
      void AddFont(const std::string& resourcePath);
      void AddArchetype(const std::string& resourcePath);
      void AddShader(const std::string& resourcePath);
      void AddSpriteSource(const std::string& resourcePath);
      void AddSoundCue(const std::string& resourcePath);
      void AddBank(const std::string& resourcePath);
      void AddLevel(const std::string& resourcePath);
      void AddCollisionGroup(const std::string& resourcePath);
      void AddCollisionTable(const std::string& resourcePath);
      void AddPhysicsMaterial(const std::string& resourcePath);
      void AddZilchScript(const std::string& resourcePath);
      void AddSpriteLayer(const std::string& resourcePath);
      void AddSpriteLayerOrder(const std::string& resourcePath);
      // Old add functions
      void AddFont(const std::string& fontName, FontPtr fontPtr);
      void AddArchetype(const std::string& archetypeName, ArchetypePtr archetypePtr);
      void AddShader(const std::string& shaderName, ShaderPtr shaderPtr);
      void AddSpriteSource(const std::string& spriteSourceName, SpriteSourcePtr spriteSourcePtr);
      void AddBank(const std::string& bankName, BankPtr bankPtr);
      void AddLevel(const std::string& levelName, LevelPtr levelPtr);
      void AddCollisionGroup(const std::string& collisionGroupName, CollisionGroupPtr collisionGroupPtr);
      void AddCollisionTable(const std::string& collisionTableName, CollisionTablePtr collisionTablePtr);
      void AddPhysicsMaterial(const std::string& physicsMaterialName, PhysicsMaterialPtr physicsMaterialPtr);
      void AddAndLoadZilchScript(const std::string& zilchScriptName, ZilchScriptPtr zilchScriptPtr);
      void AddSpriteLayer(const std::string& name, SpriteLayerPtr ptr);
      void AddSpriteLayerOrder(const std::string& name, SpriteLayerOrderPtr ptr);
      void AddTexture(const std::string& name, TexturePtr ptr);
      // Core functions
      Content(ContentConfig& config);
      void Initialize();
      void Subscribe();
      void Update(float dt);
      void Terminate();
      // Loading functions
      void LoadCoreAssets(); //!< Load default content files for the engine.   
      void LoadProjectAssets(); //!< Load the assets used by the loaded project.      
      void GenerateDefaultResources();
      void LoadAllResources();
      void LoadProjectData(const std::string&);
      // Events
      void OnContentFileMoved(Events::ContentFileMoved* event);
      void OnContentFileUpdated(Events::ContentFileUpdated* event);
      void OnContentFileDeleted(Events::ContentFileDeleted* event);
      void OnContentFileFound(Events::ContentFileFound* event);
      void OnContentFileScanComplete(Events::ContentFileScanComplete* event);
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
    inline void Content::AddResourceToMap(const std::string & resourceName, ResourcePtr ptr, ResourceMap& map)
    {
      // Prevent duplicates
      if (map.count(resourceName)) {
        // Overwrite the current level
        map.erase(resourceName);
        if (Debug::TraceFactoryResourceAdd)
          DCTrace << "Content::AddResourceToMap - " << resourceName << " is already present in the map. Overwriting. \n";
        //return;
      }

      map.insert(std::pair<std::string, ResourcePtr>(resourceName, ptr));
      if (Debug::TraceFactoryResourceAdd)
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
    inline ResourcePtr Content::getResource(const std::string & resourceName, ResourceMap map, std::string& defaultResource)
    {
      // Check if the resource is present in the map
      if (!map.count(resourceName)) {
        if (defaultResource.empty())
          return nullptr;
        else
          return map.at(defaultResource);
      }
      // Then return it
      return map.at(resourceName);
    }

  }

}

