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
    Content::Content(ContentConfig& config) : System(std::string("ContentSystem"), EnumeratedSystem::Content),
      Settings(config), Loading(false) {
      ProjectInfo.reset(new ProjectProperties());
    }

    /**************************************************************************/
    /*!
    @brief Destructor.
    */
    /**************************************************************************/
    Content::~Content()
    {
      if (LoadingThread.joinable())
        LoadingThread.join();
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

      // Load every script..
      for (auto& script : MapZilchScript) {
        script.second->Load();
        script.second->IncludeScript();
      }
      Daisy->getSystem<Reflection>()->Handler()->CompileScripts();
      
      // Load the project's graphical resources on a separate thread
      if (LoadingThread.joinable())
        LoadingThread.join();

      LoadingThread = std::thread(&Content::LoadGraphicalResources, this);


      // Load its texture onto the graphics system
      //for (auto& spriteSource : MapSpriteSource) {
      //  spriteSource.second->Load();
      //  spriteSource.second->LoadTexture();
      //}



      // Generate every SpriteSource's texture
      for (auto& spriteSource : MapSpriteSource) {
        //spriteSource.second->GenerateTexture();
      }
      
      DCTrace << " Content::LoadAllResources: '" << ProjectInfo->ProjectName << "' is done loading. \n";

    }

    /**************************************************************************/
    /*!
    @brief Loads graphical resources from file.
    */
    /**************************************************************************/
    void Content::LoadGraphicalResources()
    {
      // Load every SpriteSource's image from file
      for (auto& spriteSource : MapSpriteSource) {
        spriteSource.second->Load();
        spriteSource.second->LoadImageFromFile();
        // Add it to the queue of assets ready to be loaded by the graphics system
        std::lock_guard<std::mutex> lock(LoadedGraphicalResourcesQueue.AssetsLock);
        LoadedGraphicalResourcesQueue.Assets.push(spriteSource.second.get());
      }

      // Load every Font
      for (auto& font : MapFont) {
        font.second->Load();
        font.second->LoadFontFromFile();
        // Add it to the queue of assets ready to be loaded by the graphics system
        std::lock_guard<std::mutex> lock(LoadedGraphicalResourcesQueue.AssetsLock);
        LoadedGraphicalResourcesQueue.Assets.push(font.second.get());

        // font.second->GenerateFont();
        //font.second->Add();
      }

      //if (LoadingThread.joinable()) LoadingThread.join();
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
      // Signal that we are currently loading the project
      Loading = true;

      // Deserialize the project data
      ProjectInfo.reset(new ProjectProperties);
      std::string projectDataString;
      bool worked;
      if (FileSystem::FileReadToString(projectDataPath, projectDataString))
        worked = Serialization::Deserialize(ProjectInfo.get(), projectDataString);

      LoadProjectResources();
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
      auto LevelPath = Settings.DefaultAssetPath + "Levels/";

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
    @brief  Loads all of the project's resources. When they are done loading, 
            it will send an event.
    */
    /**************************************************************************/
    void Content::LoadProjectResources()
    {
      DCTrace << "Content::LoadProjectResources - \n";

      // Scan for the project's resources
      ScanResources();
      // Load the resources
      LoadAllResources();
      // Start the file scanner on the current project
      bool scanning = true;
      if (scanning) {
        auto settings = FileScanner::FSSettings();
        settings.DirectoryPath = ProjectInfo->ProjectPath + ProjectInfo->ResourcePath;
        settings.Frequency = 1;
        ProjectScanner.reset(new FileScanner(settings));
        ProjectScanner->Initialize();
      }

      // Signal that we are done loading the project
      Loading = false;

      // Announce that it's been loaded
      DispatchSystemEvents::ContentProjectLoaded();


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
