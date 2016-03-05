/******************************************************************************/
/*!
@file   EditorPreferences.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/4/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "EditorPreferences.h"

namespace DCEngine {
  namespace Systems {
        
    /**************************************************************************/
    /*!
    \brief  EditorPreferences constructor.
    */
    /**************************************************************************/
    EditorPreferences::EditorPreferences(Editor & editor) : EditorModule(editor, true)
    {
    }

    /**************************************************************************/
    /*!
    \brief  EditorPreferences destructor.
    */
    /**************************************************************************/
    EditorPreferences::~EditorPreferences()
    {
    }

    void EditorPreferences::Update()
    {
    }

    /**************************************************************************/
    /*!
    \brief  Displays the EditorPreferences window.
    */
    /**************************************************************************/
    void EditorPreferences::Display()
    {
      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);
      ImGui::Begin("Preferences", &WindowEnabled);

      //ImGui::PushStyleVar()
      ImGui::Columns(2, "WindowPreferences");

      // Left Column
      //ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 0));
      if (ImGui::CollapsingHeader("Editor")) CurrentTab = Tab::Editor;
      if (ImGui::CollapsingHeader("Graphics")) CurrentTab = Tab::Graphics;
      if (ImGui::CollapsingHeader("Audio")) CurrentTab = Tab::Audio;
      if (ImGui::CollapsingHeader("Style")) CurrentTab = Tab::Style;
      //ImGui::PopStyleVar();

      //ImGui::Separator();
      //// Saves before exiting
      //if (ImGui::Button("OK")) {
      //  Save();
      //  WindowEnabled = false;
      //}
      //// Exits without saving
      //ImGui::SameLine();
      //if (ImGui::Button("Cancel")) {
      //  WindowEnabled = false;
      //}


      ImGui::NextColumn();
      // Right Column

      DisplayTab();



      ImGui::End();
    }

    void EditorPreferences::DisplayTab()
    {
      switch (CurrentTab) {
      case Tab::Editor:
        TabEditor();
        break;
      case Tab::Graphics:
        TabGraphics();
        break;
      case Tab::Audio:
        TabAudio();
        break;
      case Tab::Style:
        TabStyle();
        break;
      }
    }


    void EditorPreferences::TabEditor()
    {
      ImGui::Text("Hello :P");
      ImGui::Text("Hello :P"); ImGui::Text("Hello :P"); ImGui::Text("Hello :P"); ImGui::Text("Hello :P");
      ImGui::Text("Hello :P");
      ImGui::Text("Hello :P");
      ImGui::Text("Hello :P");
    }

    void EditorPreferences::TabStyle()
    {
      ImGui::ShowStyleEditor();
    }

    void EditorPreferences::TabGraphics()
    {

    }

    void EditorPreferences::TabAudio()
    {

    }

    void EditorPreferences::Save()
    {
      DispatchSystemEvents::EngineSaveConfigurations();
    }


  }
}