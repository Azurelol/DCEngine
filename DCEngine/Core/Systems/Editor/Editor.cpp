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

    static Editor* Reference = nullptr;

    /**************************************************************************/
    /*!
    \brief  Editor Constructor.
    */
    /**************************************************************************/
    Editor::Editor(EditorConfig& settings) : System(std::string("EditorSystem"), EnumeratedSystem::Editor), 
                                                          Settings(settings)
    {      
      // Whether the Editor should be enabled right away.
      Active = Settings.Enabled;
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

      // Subscribe to project loaded events
      Daisy->Connect<Events::ContentProjectLoaded>(&Editor::OnContentProjectLoadedEvent, this);
      // Store a reference to the Reflection System
      ReflectionSystem = Daisy->getSystem<Reflection>();
      // Subscribe to events
      Subscribe();
      // Set the default space for the editor to work on
      CurrentSpace = Daisy->getGameSession()->getDefaultSpace();

      DispatchSystemEvents::EditorInitialize(*this);
   
    }

    /**************************************************************************/
    /*!
    \brief  Subscribe to events.
    */
    /**************************************************************************/
    void Editor::Subscribe()
    {
      if (Settings.Enabled) {
        Daisy->Connect<Events::MouseDown>(Daisy->getMouse(), &Editor::OnMouseDownEvent, this);
        Daisy->Connect<Events::MouseUp>(Daisy->getMouse(), &Editor::OnMouseUpEvent, this);
        Daisy->Connect<Events::MouseUpdate>(Daisy->getMouse(), &Editor::OnMouseUpdateEvent, this);
        Daisy->Connect<Events::KeyDown>(Daisy->getKeyboard(), &Editor::OnKeyDownEvent, this);
      }

      Daisy->Connect<Events::EditorEnabled>(&Editor::OnEditorEnabledEvent, this);
      Daisy->Connect<Events::SpaceInitialized>(Daisy->getGameSession(), &Editor::OnSpaceInitializedEvent, this);
    }

    /**************************************************************************/
    /*!
    \brief  Event received when a space has been initialized
    */
    /**************************************************************************/
    void Editor::OnSpaceInitializedEvent(Events::SpaceInitialized * event)
    {
      // Once the default space has been initialized, set the editor camera
      if (event->Name == CurrentSpace->Name()) {
        Launcher.SetEditorCamera(true);
      }
    }


    /**************************************************************************/
    /*!
    \brief  Event received when the current project has been loaded.
    */
    /**************************************************************************/
    void Editor::OnContentProjectLoadedEvent(Events::ContentProjectLoaded * event)
    {

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

      
    }

    /**************************************************************************/
    /*!
    \brief  Toggled the Editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor()
    {
      DCTrace << "Editor::ToggleEditor \n";
      
      // Toggle
      Active = !Active;

      // If enabling the editor...
      if (Active) {
        Launcher.Launch();
        Projects.ReloadLevel();
      }
      // If closing it...
      else {
        Launcher.Close();    
        CurrentSpace->ReloadLevel();
      }

      Deselect();
    }
    
    /**************************************************************************/
    /*!
    @brief Static interface for the Editor.
    @return A reference to the Editor system.
    */
    /**************************************************************************/
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

      if (!Active)
        return;

      WindowSplashScreen();
      if (Windows.SplashScreenEnabled)
        return;

      DrawGrid();
      DrawSelection();
      DrawMultiSelect();

      for (auto& module : ActiveModules)
        module->Display();

      // Display the editor tools
      DisplayTool();
      // Display all known editor windows
      DisplayMainMenuBar();
      WidgetLevel();
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
      Access().Projects.AutoSave();
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
      auto textureSize = ImVec2(static_cast<float>(texture.Width * imageSizeScalar),
                                static_cast<float>(texture.Height * imageSizeScalar));

      auto windowDim = Daisy->getSystem<Window>()->getWindowDimensions();
      auto splashPos = ImVec2((windowDim.x / 2) - textureSize.x / 2, (windowDim.y / 2) - textureSize.y / 2);

      ImGui::SetNextWindowPos(splashPos);
      
      
      //ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
      if (ImGui::Begin("Example: Fixed Overlay", &Windows.SplashScreenEnabled, ImVec2(0, 0), 1.0f,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings 
        | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize))
      {
        // Display splash image
        ImColor tint(255, 255, 255, 255);
        ImColor border(255, 255, 255, 0);
        ImVec2 uv0(0, 1);
        ImVec2 uv1(1, 0);
        ImGui::Image((void*)(texture.TextureID), ImVec2(textureSize.x, textureSize.y),
          uv0, uv1, tint, border);
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