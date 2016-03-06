/******************************************************************************/
/*!
@file   Editor.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/24/2015
@brief  The editor allows the real-time editing of gameobjects in levels ,
then doing serialization of the levels.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor.
    */
    /**************************************************************************/
    Editor::Editor(EditorConfig& settings) : System(std::string("EditorSystem"), EnumeratedSystem::Editor), 
                                                          Settings(settings), 
                                                          Creator(*this), Resources(*this), Archetypes(*this),
                                                          Projects(*this), Diagnostics(*this), TextEditor(*this),
                                                          Inspector(*this), Preferences(*this)
    {      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the Editor system.
    */
    /**************************************************************************/
    void Editor::Initialize()
    {
      //auto& a = Settings;

      if (TRACE_INITIALIZE)
        DCTrace << "Editor::Initialize \n";
      // Store a reference to the Reflection System
      ReflectionSystem = Daisy->getSystem<Reflection>();
      // Subscribe to events
      Subscribe();
      // Set the default space for the editor to work on
      CurrentSpace = Daisy->getGameSession()->getDefaultSpace();
      DispatchSystemEvents::EditorInitialize();
      
    }

    /**************************************************************************/
    /*!
    \brief  Subscribe to events.
    */
    /**************************************************************************/
    void Editor::Subscribe()
    {
      Daisy->Connect<Events::MouseDown>(Daisy->getMouse(), &Editor::OnMouseDownEvent, this);
      Daisy->Connect<Events::MouseUp>(Daisy->getMouse(), &Editor::OnMouseUpEvent, this);
      Daisy->Connect<Events::MouseUpdate>(Daisy->getMouse(), &Editor::OnMouseUpdateEvent, this);
      Daisy->Connect<Events::KeyDown>(Daisy->getKeyboard(), &Editor::OnKeyDownEvent, this);
      Daisy->Connect<Events::EditorEnabled>(&Editor::OnEditorEnabledEvent, this);
    }

    /**************************************************************************/
    /*!
    \brief  Event received by Input.
    */
    /**************************************************************************/
    void Editor::OnEditorEnabledEvent(Events::EditorEnabled * event)
    {
      DCTrace << "Editor::OnEditorEnabledEvent \n";
      ToggleEditor();
    }


    /**************************************************************************/
    /*!
    \brief  Updates the Editor system.
    */
    /**************************************************************************/
    void Editor::Update(float dt)
    {
      // Only enable the editor while the GUI is enabled
      if (!GUI_ENABLED)
        return;

      SystemTimer profile(this->Name());
     
      if (TRACE_UPDATE)
        DCTrace << "Editor::Update \n";
      DisplayEditor(); 
      DisplayTool();     

      
    }

    /**************************************************************************/
    /*!
    @brief  Sets the current status of the editor.
    */
    /**************************************************************************/
    void Editor::setEnabled(bool set)
    {
      Settings.EditorEnabled = set;
      DCTrace << "Editor::setEnabled : " << std::boolalpha << Settings.EditorEnabled << "\n";
    }


    /**************************************************************************/
    /*!
    @brief  Returns the current state of the editor.
    */
    /**************************************************************************/
    bool Editor::IsEnabled()
    {
      return Settings.EditorEnabled;
    }

    /**************************************************************************/
    /*!
    \brief  Toggled the Editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor()
    {
      DCTrace << "*!* Editor::ToggleEditor \n";
      // Resize the viewport to accomodate the editor
      //ApplyEditorWindowLayout();
      
      // Toggle on the editor
      setEnabled(!Settings.EditorEnabled);
      ToggleEditor(Settings.EditorEnabled);      
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor(bool toggle)
    {
      // Editor ON
      if (toggle) {
        setEnabled(true);
        // Pause the engine (Physics, Input, Events)
        DispatchSystemEvents::EnginePause();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EnginePaused' event \n";
        // Quit the Game
        DispatchGameEvents::GameEnded();
        // Reload the level
        ReloadLevel();
        // Toggle the widgets
        Windows.LibraryEnabled = true;
        Windows.ObjectsEnabled = true;
        // Clear previous commands
        Settings.Commands.CommandsCurrent.clear();
        Settings.Commands.CommandsUndo.clear();
      }
      // Editor OFF
      else {
        setEnabled(false);
        // Unpause the engine (Physics, Input, Events)
        DispatchSystemEvents::EngineResume();
        // Send the game start event
        DispatchGameEvents::GameStarted();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EngineResume' event \n";
        // Set the editor camera
        SetEditorCamera(false);
        Deselect();
        // Ask the space to reload the level
        CurrentSpace->ReloadLevel();
        
        
      }
    }

    Editor & Editor::Access()
    {
      return *Daisy->getSystem<Systems::Editor>();
    }

    /**************************************************************************/
    /*!
    @brief  Switches to a new editor tool.
    @todo   Change this to the new tool system.
    */
    /**************************************************************************/
    void Editor::SwitchTool(EditorTools tool)
    {
      ActiveTool = tool;
      return;

      switch (tool) {
      case EditorTools::None:
        break;

      case EditorTools::Translate:        
        break;

      case EditorTools::Rotate:
        break;

      case EditorTools::Scale:
        break;

      }

      //ActiveTool.reset();
      //ActiveTool = tool;
    }

    /**************************************************************************/
    /*!
    @brief Checks whether the object falls within the boundary of the currently
           active tool.
    */
    /**************************************************************************/
    bool Editor::IsToolRegion(GameObjectPtr)
    {
      return false;
    }

    /**************************************************************************/
    /*!
    @brief  Displays every created Editor window.
    */
    /**************************************************************************/
    void Editor::DisplayEditor()
    {
      // Allow diagnostics even without the editor!
      Diagnostics.Display();
      WindowConsole();

      if (!Settings.EditorEnabled)
        return;

      WindowSplashScreen();
      if (Windows.SplashScreenEnabled)
        return;

      DrawGrid();
      DrawSelection();
      DrawMultiSelect();

      for (auto& module : ActiveModules)
        module->Display();

      // Display all known editor windows
      DisplayMainMenuBar();
      WidgetLevel();
      WindowObjects();
      WindowLibrary();
      WindowSaveLevel();
      WindowLoadLevel();    
      WindowCreateFromArchetype();
      WindowTools();
      WindowAddResource();
      WindowCollisionTableEditor();
      WindowSpriteLayerOrderEditor();
      WindowCommands();
      
      // Autosave
      AutoSave();
    }

    /**************************************************************************/
    /*!
    \brief  Allows the user a variety of settings regarding levels, such as
            reloading the current level, loading a new one, or saving the
            current one.
    */
    /**************************************************************************/
    void Editor::WidgetLevel()
    {
    }


    /**************************************************************************/
    /*!
    \brief  Terminates the editor system.
    */
    /**************************************************************************/
    void Editor::Terminate()
    {
      DCTrace << "Editor::Terminate \n";
    }

    /**************************************************************************/
    /*!
    \brief  Displays the SplashScreen.
    */
    /**************************************************************************/
    void Editor::WindowSplashScreen()
    {
      if (!Windows.SplashScreenEnabled)
        return;


      auto texture = Daisy->getSystem<Content>()->getSpriteSrc("EngineLogo")->getTexture();
      auto imageSizeScalar = 2;
      auto textureSize = ImVec2(texture.Width * imageSizeScalar, texture.Height * imageSizeScalar);

      // Open the PopUp
      //ImGui::SetNextWindowSize(ImVec2(150, 150), ImGuiSetCond_FirstUseEver);
      //ImGui::OpenPopup("Welcome!");

      //if (ImGui::BeginPopupModal("Welcome")) {
      //  ImGui::Text("Hi");
      //  // Display splash image
      //  ImGui::Image((void*)(texture.TextureID), ImVec2(texture.Width, texture.Height),
      //    ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
      //  if (ImGui::IsMouseClicked(0)) {
      //    ImGui::CloseCurrentPopup();
      //  }

      //  ImGui::EndPopup();
      //}

      auto windowDim = Daisy->getSystem<Window>()->getWindowDimensions();
      auto splashPos = ImVec2((windowDim.x / 2) - textureSize.x / 2, (windowDim.y / 2) - textureSize.y / 2);

      ImGui::SetNextWindowPos(splashPos);
      //ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
      if (ImGui::Begin("Example: Fixed Overlay", &Windows.SplashScreenEnabled, ImVec2(0, 0), 0.3f,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
      {
        // Display splash image
        ImGui::Image((void*)(texture.TextureID), ImVec2(textureSize.x, textureSize.y),
          ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        if (ImGui::IsMouseClicked(0)) {
          Windows.SplashScreenEnabled = false;
        }
        //if (ImGui::Button("Continue")) {
        //}
      }
      ImGui::End();

    }

    



  }
}