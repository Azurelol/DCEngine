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
    EditorTextEditor::EditorTextEditor(Editor & editor) : EditorModule(editor, true),
                                                          CurrentScript(nullptr), CurrentShader(nullptr)
    {
      Daisy->Connect<Events::EditorSave>(&EditorTextEditor::OnEditorSaveEvent, this);
      Daisy->Connect<Events::ScriptingErrorMessage>(&EditorTextEditor::OnScriptingErrorMessageEvent, this);
      Daisy->Connect<Events::GraphicsCompileShadersError>(&EditorTextEditor::OnGraphicsCompileShadersErrorEvent, this);
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

      ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_Always);
      if (ImGui::Begin(Title.c_str(), &WindowEnabled, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)) {
        
        CheckInputs();

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
        ImGui::InputTextMultiline("##source", Text, IM_ARRAYSIZE(Text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 48),
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
      Title = script->Name() + " - Text Editor";
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

      std::string typeAsString;
      switch (type) {
      case Shader::Type::Vertex:
        typeAsString = "Vertex";
        break;
      case Shader::Type::Fragment:
        typeAsString = "Fragment";
        break;
      case Shader::Type::Geometry:
        typeAsString = "Geometry";
        break;
      }

      Title = shader->Name() + " : " + typeAsString + " - Text Editor";
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
        // Mark Zilch components for rebuilding on the next frame before we recompile them
        //DispatchSystemEvents::EditorRebuildZilchComponents();
        EditorRef.Creator.RebuildAllObjectsOnSpace();
        DispatchSystemEvents::ScriptingCompile();
      }
      else if (CurrentShader) {
        DCTrace << "EditorTextEditor::Save: '" << CurrentShader->Name() << "' \n";
        CurrentShader->Save(std::string(Text), CurrentShaderType);
        DispatchSystemEvents::GraphicsCompileShaders();
      }    
    }

    /**************************************************************************/
    /*!
    @brief Checks for specific inputs while the TextEditor window is open.
    */
    /**************************************************************************/
    void EditorTextEditor::CheckInputs()
    {
      auto io = ImGui::GetIO();
      if (io.KeyCtrl) {
        if (ImGui::IsKeyPressed(ImGuiKey_S))
          DispatchSystemEvents::EditorSave();
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

    /**************************************************************************/
    /*!
    @brief Once a script has failed to compile, display the message.. roughly!
    */
    /**************************************************************************/
    void EditorTextEditor::OnScriptingErrorMessageEvent(Events::ScriptingErrorMessage * event)
    {
      if (!CurrentScript)
        return;

      Windows::PopUpData data;
      data.Title = "'" + CurrentScript->Name() + "' has failed to compile!";
      data.Message = event->Message;
      data.Confirmation = "Back";
      auto popUp = WindowPtr(new Windows::PopUp(data));
      GUI::Add(popUp);
    }

    /**************************************************************************/
    /*!
    @brief Once a shader has failed to compile, display the message.. roughly!
    */
    /**************************************************************************/
    void EditorTextEditor::OnGraphicsCompileShadersErrorEvent(Events::GraphicsCompileShadersError * event)
    {
      if (!CurrentShader)
        return;

      Windows::PopUpData data;
      data.Title = "'" + CurrentShader->Name() + "' has failed to compile!";
      data.Message = event->ErrorMessage;
      data.Confirmation = "Back";
      auto popUp = WindowPtr(new Windows::PopUp(data));
      GUI::Add(popUp);
    }

  }
}