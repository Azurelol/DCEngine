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

    // Display a Sprite on the current space
    if (enable) {

      // Progress
      auto progressText = Systems::Editor::Access().Creator.CreateSpriteText();
      progressText->AddComponentByName("LoadingScreen");
      progressText->getComponent<Components::LoadingScreen>()->Initialize();

      // Control object

      // Background
      //auto control = Systems::Editor::Access().Creator.CreateSprite();
      //control->getComponent<Components::Sprite>()->setSpriteSource("Wow");
      //control->getComponent<Components::Transform>()->setScale(Vec3(10, 10, 1));

      
    }
    // Remove it
    else {
      //Systems::Editor::Access().SetEditorCamera(false);
      //sprite->Destroy();
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
    Configurations.Content.DefaultAssetPath = EngineConfiguration->AssetPath; // temp
    Systems.push_back(SystemPtr(new Systems::Content(Configurations.Content)));
    Systems.push_back(SystemPtr(new Systems::Reflection));
    Systems.push_back(SystemPtr(new Systems::Factory));
    Systems.push_back(SystemPtr(new Systems::Window(Configurations.Graphics, EngineConfiguration->Caption)));
    Systems.push_back(SystemPtr(new Systems::Input));
    // Editor configuration @todo change me next!
    Configurations.Editor.EditorEnabled = EngineConfiguration->EditorEnabled;
    Configurations.Editor.ProjectsPath = EngineConfiguration->ProjectsPath;
    Configurations.Editor.RecentProject = EngineConfiguration->RecentProject;
    // Add the systems to the engine's systems container
    Systems.push_back(SystemPtr(new Systems::Editor(Configurations.Editor)));
    Systems.push_back(SystemPtr(new Systems::Physics));
    Systems.push_back(SystemPtr(new Systems::Audio(Configurations.Audio)));
    Systems.push_back(SystemPtr(new Systems::Graphics(Configurations.Graphics)));
    Systems.push_back(SystemPtr(new Systems::GUI(Configurations.GUI)));
  }


  /**************************************************************************/
  /*!
  \brief Loads the project, using serialization to load all the project data.
  \param The name of the project file.
  */
  /**************************************************************************/
  //void Engine::LoadProject(std::string & projectFile) {

  //  getSystem<Systems::Content>()->LoadProjectData(projectFile);

  //  DCTrace << "\n[Engine::LoadProject - Loading " << projectFile << "]\n";

  //  // Load all resources, both defaults and project-specific
  //  getSystem<Systems::Content>()->LoadAllResources();
  //}

  //void Engine::StartProject()
  //{
  //  // Create the default gamesession object, the "game" itself,  which contains all spaces.
  //  CurrentGameSession.reset(new GameSession(_projectName));
  //  // Create the default space
  //  SpacePtr defaultSpace = CurrentGameSession->CreateSpace(_defaultSpace);
  //  // Set a reference to it in the GameSession object
  //  CurrentGameSession->DefaultSpace = defaultSpace;
  //  DCTrace << "\n[Engine::LoadProject - Finished loading " << "]\n\n";

  //  // Initialize the gamesession. (This will initialize its spaces,
  //  // and later, its gameobjects)
  //  CurrentGameSession->Initialize();
  //}


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