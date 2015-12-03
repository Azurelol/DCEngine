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
    Editor::Editor(EditorConfig settings) : System(std::string("EditorSystem"), EnumeratedSystem::Editor), 
                                                          Settings(settings)
    {      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes the Editor system.
    */
    /**************************************************************************/
    void Editor::Initialize()
    {
      if (TRACE_INITIALIZE)
        DCTrace << "Editor::Initialize \n";
      // Store a reference to the Reflection System
      ReflectionSystem = Daisy->getSystem<Reflection>();
      // Subscribe to events
      Subscribe();
      // Set the default space for the editor to work on
      CurrentSpace = Daisy->getGameSession()->getDefaultSpace();

      // If the Editor was enabled from the start,
      // toggle the widgets
      //if (EditorStart)
      //  ToggleEditor();
      

        
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
     
      if (TRACE_UPDATE)
        DCTrace << "Editor::Update \n";
      ShowSelection();
      DisplayEditor();
      UseTool();
      
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

      // Set it's current space to work on
      //CurrentSpace = Daisy->getGameSession()->getDefaultSpace();

      // Toggle on the editor
      EditorEnabled = !EditorEnabled;
      // Toggle the widgets
      WidgetLibraryEnabled = true;
      WidgetObjectsEnabled = true;

      if (EditorEnabled) {
        // Pause the engine (Physics, Input, Events)
        DispatchSystemEvents::EnginePause();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EnginePaused' event \n";
        // Reload the level
        ReloadLevel();
      }
      else {
        // Save the current level (if there's one)
        if (CurrentSpace->CurrentLevel)
          SaveLevel(CurrentSpace->CurrentLevel->Name());
        // Unpause the engine (Physics, Input, Events)
        DispatchSystemEvents::EngineResume();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EngineResume' event \n";
        // Set the editor camera
        SetEditorCamera(false);

      }
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
        EditorEnabled = true;
        // Pause the engine (Physics, Input, Events)
        DispatchSystemEvents::EnginePause();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EnginePaused' event \n";
        // Quit the Game
        DispatchGameEvents::GameEnded();
        // Reload the level
        ReloadLevel();
      }
      // Editor OFF
      else {
        EditorEnabled = false;
        // Unpause the engine (Physics, Input, Events)
        DispatchSystemEvents::EngineResume();
        // Send the game start event
        DispatchGameEvents::GameStarted();
        DCTrace << "Editor::ToggleEditor - Dispatching 'EngineResume' event \n";
        // Set the editor camera
        SetEditorCamera(false);
      }
    }

    /**************************************************************************/
    /*!
    @brief  Toggles the ImGui Test Window on and off.
    @todo   Switch to using a stack of active windows rather than this hackery.
    */
    /**************************************************************************/
    void Editor::ToggleTest()
    {
      ShowTestWindow = !ShowTestWindow;
      DCTrace << "Editor::ToggleTest : " << ShowTestWindow << "\n";
    }

    void Editor::DisplayEditor()
    {
      if (!EditorEnabled)
        return;

      // Display all known editor windows
      DisplayMainMenuBar();
      WidgetLevel();
      WidgetObjects();
      WidgetLibrary();
      WindowProperties();
      WidgetDiagnostics();
      WindowSaveLevel();
      WindowLoadLevel();    
      WindowConsole();
      WindowCreateFromArchetype();
      WindowTools();
      WindowAddResource();
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
    \brief  Allows the user to add a resource to the current project's library
            of assets.
    */
    /**************************************************************************/
    void Editor::WidgetResourceAdd()
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
      //GUIHandler->Terminate();
    }



    



  }
}