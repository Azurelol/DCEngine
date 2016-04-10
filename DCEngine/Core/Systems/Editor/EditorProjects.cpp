/******************************************************************************/
/*!
@file   EditorProjects.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorProjects.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief EditorProjects constructor.
    */
    /**************************************************************************/
    EditorProjects::EditorProjects() : EditorModule(true), WindowProjectsPropertiesEnabled(false), 
                                       Ready(false), InitializingProject(false)
    {
    }

    /**************************************************************************/
    /*!
    @brief EditorProjects destructor.
    */
    /**************************************************************************/
    EditorProjects::~EditorProjects()
    {
    }

    /**************************************************************************/
    /*!
    @brief EditorProjects initializer.
    */
    /**************************************************************************/
    void EditorProjects::Initialize()
    {
      // Create the autosave timer
      AutoSaveTimer.reset(new Time::Timer(Access().Settings.AutoSaveTime, Time::Timer::Mode::Countdown, true));

      Daisy->Connect<Events::ScriptingLibraryCompiled>(&EditorProjects::OnScriptingLibraryCompiled, this);
      Daisy->Connect<Events::ScriptingLibraryCompilationFailure>(&EditorProjects::OnScriptingLibraryCompilationFailure, this);
      Daisy->Connect<Events::ContentProjectLoaded>(&EditorProjects::OnContentProjectLoadedEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Displays the Project Properties' window.
    */
    /**************************************************************************/
    void EditorProjects::Display()
    {
      if (!WindowEnabled)
        return;

      auto title = Access().Settings.ProjectProperties->ProjectName + " Project Properties";
      ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin(title.c_str(), &WindowEnabled);
      if (ImGui::TreeNode("Game")) {
        ImGui::Checkbox("Play", &Access().Settings.ProjectProperties->Play);
        ImGui::TreePop();
      }

      ImGui::End();
    }

    /**************************************************************************/
    /*!
    @brief Attempts to load the default GameSession from archetype. If there is none,
           it will create one on the project.
    */
    /**************************************************************************/
    void EditorProjects::LoadDefaultGameSession()
    {
      auto gameSession = Access().CurrentSpace->getGameSession();

      // Revert the current GameSession to the Archetype
      if (Archetype::Find("GameSession")) {
        //Access().Archetypes.RevertToArchetype(gameSession);
      }
      // Create the Archetype on the project
      else {
        DCTrace << "EditorProjects::LoadDefaultGameSession: Creating new 'GameSession' archetype for the project.. \n";
        Access().Archetypes.UploadArchetype(gameSession);
      }

    }

    /**************************************************************************/
    /*!
    @brief Attempts to load the default Space from archetype. If there is none,
           it will create one on the project.
    */
    /**************************************************************************/
    void EditorProjects::LoadDefaultSpace()
    {
      auto& space = Access().CurrentSpace;

      // Revert the current GameSession to the Archetype
      if (Archetype::Find("Space")) {
        //Access().Archetypes.RevertToArchetype(space);
      }
      // Create the Archetype on the project
      else {
        DCTrace << "EditorProjects::LoadDefaultSpace: Creating new 'Space' archetype for the project.. \n";
        Access().Archetypes.UploadArchetype(space);
      }

    }

    /**************************************************************************/
    /*!
    @brief Initializes the current project. This will first attempt to compile
           all scripts, then if they compile successfully load the default level.
    */
    /**************************************************************************/
    void EditorProjects::InitializeProject()
    {
      InitializingProject = true;
      // Request scripts to be compiled
      DispatchSystemEvents::ScriptingCompile();
    }

    /**************************************************************************/
    /*!
    @brief  Loads the selected project into the engine.
    @param  path The path of the project.
    */
    /**************************************************************************/
    void EditorProjects::LoadProject(const std::string& path)
    {
      // Disable the editor while loading a project
      Access().Settings.EditorEnabled = false;

      // Load the project's data into the Content system. This will
      // automatically load its resources/assets for use.
      Daisy->getSystem<Content>()->LoadProject(path);
    }


    /**************************************************************************/
    /*!
    @brief  Event received when the project has finished loading all its resources.
    @param  event The event object.
    */
    /**************************************************************************/
    void EditorProjects::OnContentProjectLoadedEvent(Events::ContentProjectLoaded * event)
    {
      // Destroy the current editor camera
      Access().SetEditorCamera(false);
      // Turn on the editor
      //Access().Settings.EditorEnabled = true;
      // Save a pointer to the project data
      Access().Settings.ProjectProperties = &Daisy->getSystem<Content>()->getProjectInfo();
      // Update the window caption to display the current project
      auto projectName = Access().Settings.ProjectProperties->ProjectName;
      DCTrace << "Editor::LoadProject - Opening: " << projectName << "\n";
      DispatchSystemEvents::SetWindowCaption(projectName + "- Daisy Chain Engine");
      // Check that the scripting library has been compiled successfully before
      // trying to load the default level.
      InitializeProject();
    }

    /**************************************************************************/
    /*!
    @brief Saves the current project's properties..
    */
    /**************************************************************************/
    void EditorProjects::SaveProject()
    { 
      // Serialize the current properties to a string.
      std::string serializedData;
      if (!Serialization::Serialize(Access().Settings.ProjectProperties, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to save the serialized data! \n";
        return;
      }
      // Write to the file!
      auto& path = Access().Settings.ProjectProperties->ProjectPath + Access().Settings.ProjectProperties->ProjectName + ".dceproj";
      if (!FileSystem::FileWriteString(path, serializedData)) {
        DCTrace << "EditorProjects::SaveProject: Failed to write to the project properties file! \n";
        return;
      }
      DCTrace << "EditorProjects::SaveProject: Saved the current project! \n";
    }

    /**************************************************************************/
    /*!
    @brief  Autosaves the current project.
    */
    /**************************************************************************/
    void EditorProjects::AutoSave()
    {
      // The timer's update will return true every 60 seconds. When it does so,
      // we will automatically save the current level.
      if (AutoSaveTimer->Update()) {
        DCTrace << "Editor::AutoSave - Autosaving... \n";
        SaveCurrentLevel();
        SaveLevelBackup();
      }
    }


    /**************************************************************************/
    /*!
    @brief  Creates a backup of the current level.
    */
    /**************************************************************************/
    void EditorProjects::SaveLevelBackup()
    {

    }

    /**************************************************************************/
    /*!
    @brief  Loads the level, deserializing a level file's binary data and
            constructing gameobjects along with their components.
    */
    /**************************************************************************/
    bool EditorProjects::LoadLevel(std::string level)
    {
      // Only attempt to load a level if the scripting library is currently working
      if (!Ready) {
        // Launch a popup!
        return false;
      }

      DCTrace << "Editor::LoadLevel - Loading " << level << "\n";

      // Save the currently-loaded level before loading a new one
      if (Access().CurrentSpace->getCurrentLevel()) {
        auto currentLevelName = Access().CurrentSpace->getCurrentLevel()->Name();
        SaveLevel(currentLevelName);
      }

      // Query the Content system for the Level resource 
      auto levelPtr = Daisy->getSystem<Systems::Content>()->getLevel(level);
      Access().Deselect();

      if (levelPtr == nullptr) {
        DCTrace << "Editor::LoadLevel - Could not find " << level << "\n";
        return false;
      }

      // Deselect
      Access().Deselect();
      // Load the level
      Access().CurrentSpace->LoadLevel(level);
      // If the level doesn't have a levelsettings object (legacy), add one!
      if (!Access().CurrentSpace->FindObjectByName("LevelSettings")) {
        Access().Creator.CreateLevelSettings();
      }
      
      // Load the editor camera
      Access().SetEditorCamera(true);
      // Sets the current window's caption
      Access().UpdateCaption();
      // Set it as the default level
      Access().Settings.ProjectProperties->DefaultLevel = level;
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Saves the current level, serializing all of the active gameobjects
    along with their components in the space.
    @param  level The name of the level.
    @todo   The level path is currently hardcoded. Change that.
    */
    /**************************************************************************/
    bool EditorProjects::SaveLevel(std::string level)
    {
      DCTrace << "Editor::SaveLevel - Saving " << level << "\n";

      // Get the current project's path
      std::string LevelPath = Access().Settings.ProjectProperties->ProjectPath + Access().Settings.ProjectProperties->ResourcePath;
      // ("Projects/Rebound/Resources/Levels/");
      auto levelResource = Access().CurrentSpace->SaveLevel(LevelPath + level + Level::Extension());

      // If the level was saved successfully
      if (levelResource) {
        // Set it as the current level on the space
        Access().CurrentSpace->setCurrentLevel(levelResource);
        // Scan for resources again
        //Daisy->getSystem<Content>()->ScanResources();
        Daisy->getSystem<Content>()->ScanForLevels();
        // Update the caption
        Access().UpdateCaption();

        return true;
      }
      else
        return false;
    }

    /**************************************************************************/
    /*!
    @brief Reloads the currently loaded level.
    */
    /**************************************************************************/
    bool EditorProjects::ReloadLevel()
    {
      // Reload the current level
      Access().CurrentSpace->ReloadLevel();
      // Reload the editor camera
      Access().SetEditorCamera(true);
      // Clear the currently selected object
      Access().Deselect();
      // Eh?
      return true;
    }

    /**************************************************************************/
    /*!
    @brief  Saves the currently-loaded level.
    */
    /**************************************************************************/
    void EditorProjects::SaveCurrentLevel()
    {
      if (Access().CurrentSpace->getCurrentLevel()) {
        auto currentLevelName = Access().CurrentSpace->getCurrentLevel()->Name();
        SaveLevel(currentLevelName);
      }
    }
    
    /**************************************************************************/
    /*!
    @brief If the scripting library compiles, load the default level.
    */
    /**************************************************************************/
    void EditorProjects::OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled * event)
    {
      Ready = true;

      // If the project is being initialized...
      if (InitializingProject) {
        // Load the default GameSession
        LoadDefaultGameSession();
        // Load the default Space
        LoadDefaultSpace();
        // Load its default level 
        auto play = Access().Settings.ProjectProperties->Play;
        auto load = LoadLevel(Access().Settings.ProjectProperties->DefaultLevel);

        // If the level loaded successfully
        if (load) {
          // If set to play mode, disable the editor
          if (play)
            Access().ToggleEditor(false, false);
          // Otherwise, load the editor right away
          else {
            DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
            Access().ToggleEditor(true, false);
          }

        }

        // No default level set, turn on the editor!
        else
          Access().ToggleEditor(true, true);

        InitializingProject = false;
      }

    }

    /**************************************************************************/
    /*!
    @brief If the scripting library fails to compile, do nothing!
    */
    /**************************************************************************/
    void EditorProjects::OnScriptingLibraryCompilationFailure(Events::ScriptingLibraryCompilationFailure * event)
    {
      Ready = false;
      // If the project is being initialized...
      if (InitializingProject) {
        DCTrace << "Editor::LoadProject - Default level found editor turned on \n";
        Access().ToggleEditor(true);
        InitializingProject = false;
      }
    }

  }
}

