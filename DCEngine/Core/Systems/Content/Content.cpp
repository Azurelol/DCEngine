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

      // Hardcode the path momentarily
      ProjectInfo->ResourcePath = "Projects/Rebound/Resources/";
      ProjectInfo->AssetPath = "Projects/Rebound/Assets/";

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
      // Generate every SoundCue's sound
      for (auto soundCue : SoundCueMap) {
        soundCue.second->GenerateSound();
      }

      // Load every SpriteSource's texture
      for (auto spriteSource : SpriteSourceMap) {
        // Load the SpriteSource's properties data from file
        spriteSource.second->Load(); 
        // Load its texture onto the graphics system
        spriteSource.second->LoadTexture();
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
    void Content::LoadProject(std::string projectDataPath)
    {
      // Deserialize the project data
      ProjectInfo.reset(new ProjectData);
      std::string projectDataString;
      bool worked;
      if (FileSystem::FileReadToString(projectDataPath, projectDataString))
        worked = Serialization::Deserialize(ProjectInfo.get(), projectDataString);
      // Load it
      LoadProjectResources();



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

      //ScanForLevels();
      //ScanForArchetypes();
      //ScanForSoundCues();
      //ScanForSpriteSources();
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
