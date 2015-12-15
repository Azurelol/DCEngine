/******************************************************************************/
/*!
@file   ResourcesAdd.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/28/2015
@brief  Methods that add various resources to the currently active project.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"
#include "../Testing.h" // Dollhouse

namespace DCEngine {
  namespace Systems {


    /**************************************************************************/
    /*!
    @brief  Adds a resource to the project by creating it. This is normally
            done for engine-specific resources.
    @param  name The name of the resource.
    @param  type The type of the resource.
    */
    /**************************************************************************/
    void Editor::ResourceCreate(std::string& name, ResourceType type) {
      DCTrace << "Editor::ResourceCreate - Creating " << name << "\n";

      switch (type) {

      case ResourceType::Level:
        CreateLevel(name);
        break;

      default:
        break;

      }
    }

    /**************************************************************************/
    /*!
    @brief  Adds a resource to the project from a file. This is normally
            done for resources that use external assets.
    @param  name The name of the resource.
    @param  type The type of the resource.
    */
    /**************************************************************************/
    void Editor::ResourceAddFromFile(std::string & name, ResourceType type)
    {
      DCTrace << "Editor::ResourceAddFromFile - Attempting to create: " << name << "\n";

      // Open the file dialog by default on the project's resource path
      auto executablePath = boost::filesystem::initial_path().string();      
      auto resourcesPath = executablePath + "\\" + Settings.ProjectInfo->ProjectPath + Settings.ProjectInfo->AssetPath;
      FileSystem::CorrectPath(resourcesPath);      
      //auto a = boost::filesystem::
      //auto resourcesPath = executablePath + Settings.ProjectsPath + Settings.ProjectInfo->ResourcePath; // "\\Projects";
      // If the projects directory doesn't exist, start from the executable
      if (!FileSystem::Exists(resourcesPath)) {
        resourcesPath = executablePath;
      }
      // Grab the absolute path of the asset file selected
      auto assetPath = FileSystem::FileOpenDialog(resourcesPath, std::string(""));
      //auto a = boost::filesystem::path(assetPath). - boost::filesystem::initial_path();
      // Convert to the path relative to the executable
      //boost::filesystem::

      DCTrace << "Editor::ResourceAddFromFile - From asset: " << FileSystem::FileNoExtension(assetPath) << "\n";
      
      // Create the resource from file
      switch (type) {
        
      case ResourceType::SpriteSource:
        CreateSpriteSource(name, assetPath);
        break;

      case ResourceType::SoundCue:
        CreateSoundCue(name, assetPath);
        break;

      }

      // Add the resource to the project by creating a resource object
      // using the specified resource name, the asset path. This resource
      // will be serialized in the resource folder.

    }

    /**************************************************************************/
    /*!
    @brief  Creates a level on the currently selected project
    @param  The name of the level to create.
    */
    /**************************************************************************/
    bool Editor::CreateLevel(std::string & name)
    {
      DCTrace << "Editor::CreateLevel - Creating " << name << "\n";
      // Save the current level, if there's one loaded
      if (CurrentSpace->CurrentLevel)
        SaveLevel(CurrentSpace->CurrentLevel->Name());
      // Destroy the objects in the space
      CurrentSpace->DestroyAll();
      // Add a default camera
      CurrentSpace->CreateObject("Camera");      
      // Save the level
      SaveLevel(name);
      // Add the editor camera right away
      SetEditorCamera(true);
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SpriteSource resource from a given name and the
            path to where the image file lies.
    @param  name The name of the SpriteSource.
    @param  assetPath The path to the SpriteSource.
    @return A pointer to the recently-created resource object.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSpriteSource(std::string & name, std::string & assetPath)
    {
      DCTrace << "Editor::CreateSpriteSource - Created '" << name << "' with asset: '" << assetPath << "' \n";
            
      // Create a SpriteSource object
      auto spriteSourcePath = Settings.ProjectInfo->ProjectPath + Settings.ProjectInfo->ResourcePath + name + SpriteSource::Extension();
      auto spriteSource = SpriteSourcePtr(new SpriteSource(spriteSourcePath));
      // Store the path of its asset
      spriteSource->setAssetPath(assetPath);
      // Serialize it and save it to file
      auto data = spriteSource->Build();
      // Add it to the content system
      Daisy->getSystem<Content>()->AddSpriteSource(name, spriteSource);
      // Load its textures immediately
      spriteSource->LoadTexture();

      return spriteSource.get();
    }

    /**************************************************************************/
    /*!
    @brief  Creates a SoundCue resource from a given name and the
            path to where the sound file lies.
    @param  name The name of the SoundCue.
    @param  assetPath The path to the SoundCue.
    @return A pointer to the recently-created resource object.
    */
    /**************************************************************************/
    ResourcePtr Editor::CreateSoundCue(std::string & name, std::string & assetPath)
    {
      return ResourcePtr();
    }

  }
}
