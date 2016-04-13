/******************************************************************************/
/*!
\file   Engine.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The x

Functions include:
- Name
Description here.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Engine.h"


namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Deserializes the engine's configuration files.
  */
  /**************************************************************************/
  void Engine::LoadConfigurationFiles(const std::string& configFile)
  {
    // Load the engine's configuration from a file
    EngineConfiguration.reset(new EngineConfig);
    std::string configString;
    if (FileSystem::FileReadToString(configFile, configString))
      Serialization::Deserialize(EngineConfiguration.get(), configString);
    else
      DCTrace << "Engine::LoadConfigurationFiles - Failed to deserialize engine configuration! \n";

    // Load the Graphics Config
    LoadConfiguration(Configurations.Graphics, Systems::GraphicsConfig::FileName());
    // Load the Audio Config
    LoadConfiguration(Configurations.Audio, Systems::AudioConfig::FileName());
    // Load the Debug Config
    LoadConfiguration(Configurations.Debug, Systems::DebugConfig::FileName());
    // Load the GUI Config
    LoadConfiguration(Configurations.GUI, Systems::GUIConfig::FileName());
    // Load the Editor Config
    LoadConfiguration(Configurations.Editor, Systems::EditorConfig::FileName());
    // Load the Content Config
    LoadConfiguration(Configurations.Content, Systems::ContentConfig::FileName());

  }

  /**************************************************************************/
  /*!
  @brief Displays the current loading screen.
  */
  /**************************************************************************/
  void Engine::DisplayLoadingScreen(bool enable)
  {
    DCTrace << __FUNCTION__ << " \n";

    // The loading screen object
    //static GameObjectPtr sprite;

    bool loadLevel = true;

    // Display a Sprite on the current space
    if (enable) {

      if (loadLevel) {
        CurrentGameSession->DefaultSpace->LoadLevel(std::string("LoadingScreen"));
      }
      else {
        auto progressText = CurrentGameSession->DefaultSpace->CreateObject();
        progressText->AddComponentByName("LoadingScreen");
        progressText->getComponent<Components::LoadingScreen>()->Initialize();
      }      
    }

  }

  /**************************************************************************/
  /*!
  @brief  Constructs and loads all the systems that the engine uses.
  */
  /**************************************************************************/
  void Engine::ConstructSystems()
  {
    // Construct the input interface objects
    KeyboardHandle.reset(new Keyboard());
    MouseHandle.reset(new Mouse());
    // Systems are added to to the engine's systems container, and configurations passed on.
    //Configurations.Content.DefaultAssetPath = EngineConfiguration->AssetPath; // temp
    Systems.push_back(SystemPtr(new Systems::Content(Configurations.Content)));
    Systems.push_back(SystemPtr(new Systems::Reflection));
    Systems.push_back(SystemPtr(new Systems::Factory));
    Systems.push_back(SystemPtr(new Systems::Window(Configurations.Graphics, EngineConfiguration->Caption)));
    Systems.push_back(SystemPtr(new Systems::Input));
    // Editor configuration @todo change me next!
    //Configurations.Editor.ProjectsPath = EngineConfiguration->ProjectsPath;
    //Configurations.Editor.RecentProject = EngineConfiguration->RecentProject;
    // Add the systems to the engine's systems container
    Systems.push_back(SystemPtr(new Systems::Editor(Configurations.Editor)));
    Systems.push_back(SystemPtr(new Systems::Physics));
    Systems.push_back(SystemPtr(new Systems::Audio(Configurations.Audio)));
    Systems.push_back(SystemPtr(new Systems::Graphics(Configurations.Graphics)));
    Systems.push_back(SystemPtr(new Systems::GUI(Configurations.GUI)));
  }
  

  /**************************************************************************/
  /*!
  @brief  Loads the GameSession's default space
  */
  /**************************************************************************/
  void Engine::LoadDefaultSpace()
  {
    // Create the default space
    SpacePtr defaultSpace = CurrentGameSession->CreateSpace(_defaultSpace, false);
    // Set a reference to it in the GameSession object
    CurrentGameSession->DefaultSpace = defaultSpace;
  }


}