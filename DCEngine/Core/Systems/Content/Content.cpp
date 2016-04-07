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
#include "../../Engine/Engine.h"

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
      ProjectInfo.reset(new ProjectProperties());
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Content system.
    */
    /**************************************************************************/
    void Content::Initialize() {
      if (TRACE_INITIALIZE)
        DCTrace << "Content::Initialize \n";

      // Subscribe to events
      Subscribe();
      // Hardcode the path momentarily
      ProjectInfo->ResourcePath = "Projects/Rebound/Resources/";
      ProjectInfo->AssetPath = "Projects/Rebound/Assets/";
      // Load the default resources of the engine's
      LoadCoreAssets();
    }

    /**************************************************************************/
    /*!
    @brief Subscribes to events.
    */
    /**************************************************************************/
    void Content::Subscribe()
    {
      Daisy->Connect<Events::ContentFileMoved>(&Content::OnContentFileMoved, this);
      Daisy->Connect<Events::ContentFileUpdated>(&Content::OnContentFileUpdated, this);
      Daisy->Connect<Events::ContentFileDeleted>(&Content::OnContentFileDeleted, this);
      Daisy->Connect<Events::ContentFileFound>(&Content::OnContentFileFound, this);
      Daisy->Connect<Events::ContentFileScanComplete>(&Content::OnContentFileScanComplete, this);
    }


    /**************************************************************************/
    /*!
    @brief Event received when a known file has been moved.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Content::OnContentFileMoved(Events::ContentFileMoved * event)
    {
    }

    /**************************************************************************/
    /*!
    @brief Event received when a known file has been updated.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Content::OnContentFileUpdated(Events::ContentFileUpdated * event)
    {
      // If it's a ZilchScript
      if (event->Extension == ZilchScript::Extension()) {
        DCTrace << "Content::OnContentFileUpdated: Zilch script has been updated! \n";
        DispatchSystemEvents::ScriptingCompile();
      }
    }

    /**************************************************************************/
    /*!
    @brief Event received when a known file has been deleted.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Content::OnContentFileDeleted(Events::ContentFileDeleted * event)
    {
    }

    /**************************************************************************/
    /*!
    @brief Event received when a new file has been found.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Content::OnContentFileFound(Events::ContentFileFound * event)
    {
    }

    /**************************************************************************/
    /*!
    @brief Event received whena scan has been completed.
    @param event A pointer to the event.
    */
    /**************************************************************************/
    void Content::OnContentFileScanComplete(Events::ContentFileScanComplete * event)
    {
    }


    // Threaded helper
    void LoadSpriteSourceTextures(const std::vector<SpriteSourcePtr> spriteSources) {

      
      // Load its texture onto the graphics system
      for (auto& spriteSource : spriteSources) {
        spriteSource->LoadTexture();
      }
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
      DCTrace << " Content::LoadAllResources: '" << ProjectInfo->ProjectName << "' has started loading... \n";
      // Generate every SoundCue from file
      for (auto& soundCue : MapSoundCue) {
        soundCue.second->Load();
        soundCue.second->Generate();
      }
      // Load all banks from file, then add them to audio system
      for (auto& bank : MapBank) {
        bank.second->Load();
        bank.second->Add();
      }
      Daisy->getSystem<Audio>()->Generate();




      // We will create 4 threads to load images
      //AssetLoader::ALSettings alSettings(4);
      //AssetLoader spriteLoader(alSettings);
      //auto spriteSourceLoad = std::bind(&SpriteSource::LoadTexture);
      //spriteLoader.Load<SpriteSourceMap, SpriteSourcePtr>(MapSpriteSource, spriteSourceLoad);      
      
      //------------------------------------------------------------------------// 
      // Sort the spriteSources by file size
      //FileData::SortBiggest sortFunc;
      //FileQueue sortedSpriteSources(sortFunc);
      //for (auto& spriteSource : MapSpriteSource) {
      //  auto fileData = FileData(spriteSource.first, FileSystem::FileSize(spriteSource.second->getAssetPath()));
      //  sortedSpriteSources.push(fileData);
      //}
      //// Create partitions depending on the number of threads
      //size_t threadCount = 1;
      //std::vector<std::thread> threads;
      //std::vector<std::vector<SpriteSourcePtr>> partitions(threadCount);
      //int i = 0;
      //while (!sortedSpriteSources.empty()) {
      //  auto file = sortedSpriteSources.top(); sortedSpriteSources.pop();
      //  partitions[i++ % partitions.size()].push_back(MapSpriteSource.at(file.Path));
      //}       
      //// For each partition, create a tread
      //for (auto& partition : partitions) {
      //  threads.push_back(std::thread(LoadSpriteSourceTextures, partition));
      //}

      //------------------------------------------------------------------------//
      
      //// Load every SpriteSource's texture
      //for (auto& spriteSource : MapSpriteSource) {
      //  // Load the SpriteSource's properties data from file
      //  spriteSource.second->Load();
      //  spriteSource.second->LoadImageFromFile();
      //  spriteSource.second->GenerateTexture();
      //}

      
      // Load its texture onto the graphics system
      for (auto& spriteSource : MapSpriteSource) {
        spriteSource.second->Load();
        spriteSource.second->LoadTexture();
      }


      // Load every Font
      for (auto& font : MapFont) {
        font.second->Load();
        font.second->Add();
      }

      // Load every script..
      for (auto& script : MapZilchScript) {
        script.second->Load();
        script.second->IncludeScript();
      }
      Daisy->getSystem<Reflection>()->Handler()->CompileScripts();


      // Do not continue until all contents have been loaded
      //for (auto& thread : threads) {
      //  if (thread.joinable()) thread.join();
      //}

      DCTrace << " Content::LoadAllResources: '" << ProjectInfo->ProjectName << "' is done loading. \n";

    }

    /**************************************************************************/
    /*!
    @brief Deserializes a ProjectProperties file for project data settings.
    */
    /**************************************************************************/
    void Content::LoadProjectData(const std::string& projectData)
    {
      // Load the loaded project's assets
      LoadProjectAssets();

      DCTrace << "Content::LoadProjectData - Finished loading all project data. \n";
    }


    /**************************************************************************/
    /*!
    @brief  Loads a project from file.
    @param  A string containing the path of the project data file.
    */
    /**************************************************************************/
    void Content::LoadProject(const std::string& projectDataPath)
    {
      // Deserialize the project data
      ProjectInfo.reset(new ProjectProperties);
      std::string projectDataString;
      bool worked;
      if (FileSystem::FileReadToString(projectDataPath, projectDataString))
        worked = Serialization::Deserialize(ProjectInfo.get(), projectDataString);
      // Load it
      LoadProjectResources();
      // Start the file scanner on the current project
      bool scanning = true;
      if (scanning) {
        auto settings = FileScanner::FSSettings();
        settings.DirectoryPath = ProjectInfo->ProjectPath + ProjectInfo->ResourcePath;
        settings.Frequency = 1;
        ProjectScanner.reset(new FileScanner(settings));
        ProjectScanner->Initialize();
      }
      // Announce that it's been loaded
      DispatchSystemEvents::ContentProjectLoaded();

    }

    /**************************************************************************/
    /*!
    @brief  Saves the project to a file.
    @param  A string containing the path of the project data file.
    */
    /**************************************************************************/
    void Content::SaveProject(const std::string & projectPath)
    {
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

    void Content::LoadProjectResources()
    {
      DCTrace << "Content::LoadProjectResources - \n";

      // Scan for the resources... 
      ScanResources();
      // Load the resources
      LoadAllResources();

    }

    /**************************************************************************/
    /*!
    @brief Updates the Content system.
    @note
    */
    /**************************************************************************/
    void Content::Update(float dt) {
      SystemTimer profile(this->Name());
      // On every update, scan for resources being changed outside the application. (Hot loading?)
    }

    /**************************************************************************/
    /*!
    @brief Terminates the Content system.
    */
    /**************************************************************************/
    void Content::Terminate() {
      DCTrace << "Content::Terminate\n";
    }




  }
}
