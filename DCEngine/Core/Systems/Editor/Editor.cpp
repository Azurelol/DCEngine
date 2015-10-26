#include "Editor.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    \brief  Constructor.
    */
    /**************************************************************************/
    Editor::Editor() : System(std::string("EditorSystem"), EnumeratedSystem::Editor)
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
        trace << "Editor::Initialize \n";
      //GUIHandler->Initialize();
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
        trace << "Editor::Update \n";
      DisplayEditor();

      DisplayGUITest();
      
    }

    /**************************************************************************/
    /*!
    \brief  Toggled the Editor on and off.
    */
    /**************************************************************************/
    void Editor::ToggleEditor()
    {
      EditorEnabled = !EditorEnabled;
      trace << "Editor::ToggleEditor : " << EditorEnabled << "\n";
    }

    /**************************************************************************/
    /*!
    \brief  Toggles the ImGui Test Window on and off.
    */
    /**************************************************************************/
    void Editor::ToggleTest()
    {
      ShowTestWindow = !ShowTestWindow;
      trace << "Editor::ToggleTest : " << ShowTestWindow << "\n";
    }

    void Editor::DisplayEditor()
    {
      if (!EditorEnabled)
        return;

      WidgetObjects();
      WidgetLibrary();
      WidgetProperties();
    }

    /**************************************************************************/
    /*!
    \brief  Displays a list of Objects in the default space.
    */
    /**************************************************************************/
    void Editor::WidgetObjects()
    {
      // Set the position of the window
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      // Title
      ImGui::Begin("Objects", &EditorEnabled);            
      // Print every name
      auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();    
      for (auto object : *objects ) {
        auto objName = object->Name().c_str();
        // If the user has selected the GameObject.
        if (ImGui::Button(objName))
          SelectedGameObject = object.get();
        //ImGui::Text(object->Name().c_str());
      }
      // Ends the window
      ImGui::End();

    }

    /**************************************************************************/
    /*!
    \brief  Displays the properties of the currently selected object.
    */
    /**************************************************************************/
    void Editor::WidgetProperties()
    { 
      if (SelectedGameObject == nullptr)
        return;

      ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiSetCond_FirstUseEver);      
      ImGui::Begin("Properties", &EditorEnabled);

      // 1. Display the object's name
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Name: ");
      ImGui::Text(SelectedGameObject->Name().c_str());
      // 2. Display its components
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Components: ");
      for (auto component : *SelectedGameObject->AllComponents()) {
        ImGui::Text(component->Name().c_str());
      }

      ImGui::End();
    }

    void Editor::WidgetLibrary()
    {
      ImGui::SetNextWindowSize(ImVec2(200, 400), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Library", &EditorEnabled);

      // 1. Display every spritesource
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Sprites: ");
      for (auto spriteSrc : *Daisy->getSystem<Content>()->AllSpriteSources()) {
        ImGui::Text(spriteSrc.second->Name().c_str());
      }
      // 2. Display every soundcue
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "SoundCues: ");
      for (auto soundCue : *Daisy->getSystem<Content>()->AllSoundCues()) {
        ImGui::Text(soundCue.second->Name().c_str());
      }
      // 2. Display every shader
      ImGui::TextColored(ImVec4(0, 0.5, 1, 1), "Shaders: ");
      for (auto shader : *Daisy->getSystem<Content>()->AllShaders()) {
        ImGui::Text(shader.second->Name().c_str());
      }

      ImGui::End();


      //ImGui::SetNextWindowSize(ImVec2(200, 500), ImGuiSetCond_FirstUseEver);
      //// Title
      //ImGui::Begin("Library", &EditorEnabled);
      //// Print every name
      //auto objects = Daisy->getGameSession()->getDefaultSpace()->AllObjects();
      //for (auto object : *objects) {
      //  ImGui::Text(object->Name().c_str());
      //}
      //// Ends the window
      //ImGui::End();
    }

    /**************************************************************************/
    /*!
    \brief  Displays the ImGui Test Window.
    */
    /**************************************************************************/
    void Editor::DisplayGUITest()
    {
      if (ShowTestWindow) {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&ShowTestWindow);
      }
    }

    /**************************************************************************/
    /*!
    \brief  Terminates the editor system.
    */
    /**************************************************************************/
    void Editor::Terminate()
    {
      trace << "Editor::Terminate \n";
      //GUIHandler->Terminate();
    }

    void Editor::Serialize(Json::Value & root)
    {
    }

    void Editor::Deserialize(Json::Value & root)
    {
    }

  }
}