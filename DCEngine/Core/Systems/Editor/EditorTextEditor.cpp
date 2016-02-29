/******************************************************************************/
/*!
@file   EditorTextEditor.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "EditorTextEditor.h"

#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {

    // Statics
    static bool ReadOnly = false;
    static char Text[1024 * 16]; // Magic number no!

    /**************************************************************************/
    /*!
    @brief EditorTextEditor constructor.
    @param editor A reference to the Editor.
    */
    /**************************************************************************/
    EditorTextEditor::EditorTextEditor(Editor & editor) : EditorModule(editor, true)
    {
      Daisy->Connect<Events::EditorSave>(&EditorTextEditor::OnEditorSaveEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Displays the text editor.
    */
    /**************************************************************************/
    void EditorTextEditor::Display()
    {
      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(800, 800), ImGuiSetCond_FirstUseEver);
      if (ImGui::Begin("Text Editor", &WindowEnabled, ImGuiWindowFlags_MenuBar)) {

        if (ImGui::BeginMenuBar()) {
          if (ImGui::BeginMenu("File")) {
            if ((ImGui::MenuItem("Save"))) Save();
            if ((ImGui::MenuItem("Close"))) Close();

            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }


        //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ////ImGui::Checkbox("Read-only", &ReadOnly);
        //ImGui::PopStyleVar();
        ImGui::InputTextMultiline("##source", Text, IM_ARRAYSIZE(Text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
                                  ImGuiInputTextFlags_AllowTabInput | (ReadOnly ? ImGuiInputTextFlags_ReadOnly : 0));



      }
      ImGui::End();      

    }

    /**************************************************************************/
    /*!
    @brief Loads the specified script onto the text editor.
    @param script A pointer to the script resource.
    */
    /**************************************************************************/
    void EditorTextEditor::Load(ZilchScriptPtr script)
    {
      Clear();
      DCTrace << "EditorTextEditor::Load: Loading the script '" << script->Name() << "' \n";
      WindowEnabled = true;
      CurrentScript = script;
      CurrentScript->Load();
      std::strcpy(Text, CurrentScript->Read().c_str());
    }

    /**************************************************************************/
    /*!
    @brief Loads the specified shader onto the text editor.
    @param shader A pointer to the shader resource.
    */
    /**************************************************************************/
    void EditorTextEditor::Load(Shader* shader, Shader::Type type)
    {
      Clear();
      DCTrace << "EditorTextEditor::Load: Loading the shader '" << shader->Name() << "' \n";
      WindowEnabled = true;
      CurrentShader = shader;
      CurrentShaderType = type;
      std::strcpy(Text, CurrentShader->Read(type).c_str());
    }

    /**************************************************************************/
    /*!
    @brief Saves the currently selected script to a file.
    */
    /**************************************************************************/
    void EditorTextEditor::Save()
    {
      if (CurrentScript) {
        DCTrace << "EditorTextEditor::Save: '" << CurrentScript->Name() << "' \n";
        CurrentScript->Save(std::string(Text));
        DispatchSystemEvents::ScriptingCompile();
      }
      else if (CurrentShader) {
        DCTrace << "EditorTextEditor::Save: '" << CurrentShader->Name() << "' \n";
        CurrentShader->Save(std::string(Text), CurrentShaderType);
        DispatchSystemEvents::GraphicsCompileShaders();
      }    
    }

    void EditorTextEditor::Clear()
    {
      CurrentScript = nullptr;
      CurrentShader = nullptr;
    }

    /**************************************************************************/
    /*!
    @brief Closes the current file.
    @todo  Perhaps ask the user if he wants to save it with a prompt?
    */
    /**************************************************************************/
    void EditorTextEditor::Close()
    {
      //Save();
      Clear();
      WindowEnabled = false;
    }

    /**************************************************************************/
    /*!
    @brief Saves the Editor's current file if its active.
    */
    /**************************************************************************/
    void EditorTextEditor::OnEditorSaveEvent(Events::EditorSave * event)
    {
      if (!WindowEnabled)
        return;

      if (CurrentScript)
        Save();

      if (CurrentShader)
        Save();
    }

  }
}