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
    EditorTextEditor::EditorTextEditor() : EditorModule(true), CurrentScript(nullptr), CurrentShader(nullptr)
    {
    }

    /**************************************************************************/
    /*!
    @brief Initialize the TextEditor.
    */
    /**************************************************************************/
    void EditorTextEditor::Initialize()
    {
      Daisy->Connect<Events::ContentProjectLoaded>(&EditorTextEditor::OnContentProjectLoadedEvent, this);
    }


    /**************************************************************************/
    /*!
    @brief Subscribe to events when the project has been finally loaded.
    */
    /**************************************************************************/
    void EditorTextEditor::OnContentProjectLoadedEvent(Events::ContentProjectLoaded * event)
    {
      Daisy->Connect<Events::EditorSave>(&EditorTextEditor::OnEditorSaveEvent, this);
      Daisy->Connect<Events::ScriptingErrorMessage>(&EditorTextEditor::OnScriptingErrorMessageEvent, this);
      Daisy->Connect<Events::ScriptingLibraryAboutToCompile>(&EditorTextEditor::OnScriptingLibraryAboutToCompile, this);
      Daisy->Connect<Events::ScriptingLibraryCompiled>(&EditorTextEditor::OnScriptingLibraryCompiled, this);
      Daisy->Connect<Events::GraphicsCompileShadersError>(&EditorTextEditor::OnGraphicsCompileShadersErrorEvent, this);
      Daisy->Connect<Events::WindowGainedFocus>(&EditorTextEditor::OnWindowGainedFocusEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Displays the text editor.
    */
    /**************************************************************************/
    void EditorTextEditor::Display()
    {
      // Displays any current compilation errors
      DisplayCompilationErrors();

      if (!WindowEnabled)
        return;

      ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_Always);
      if (ImGui::Begin(Title.c_str(), &WindowEnabled, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoResize)) {
        
        CheckHotkeys();

        if (ImGui::BeginMenuBar()) {
          if (ImGui::BeginMenu("File")) {
            if ((ImGui::MenuItem("Open Externally", "CTRL+Tab"))) OpenOnExternalEditor();
            if ((ImGui::MenuItem("Save", "CTRL+S"))) Save();
            if ((ImGui::MenuItem("Print", "CTRL+P", false))) Print();
            if ((ImGui::MenuItem("Close"))) Close();
            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }

        ImGui::InputTextMultiline("##source", Text, IM_ARRAYSIZE(Text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 48),
                                  ImGuiInputTextFlags_AllowTabInput | (ReadOnly ? ImGuiInputTextFlags_ReadOnly : 0)); 


      }
      ImGui::End();      

      if (!WindowEnabled) 
        Clear();

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
      CurrentScript = script;

      // Loading only externally
      if (Access().Settings.TextEditorOpenExternally) {
        OpenOnExternalEditor();
        return;
      }

      // Loading internally
      WindowEnabled = true;
      Title = script->Name() + " - Text Editor";
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
    @brief Reloads the currently selected script/shader from file.
    */
    /**************************************************************************/
    void EditorTextEditor::Reload()
    {
      DCTrace << "EditorTextEditor::Reload: Recompiling... \n";
      if (CurrentShader) {
        Load(CurrentShader, CurrentShaderType);
      }
      if (CurrentScript) {
        Load(CurrentScript);
      }
    }

    /**************************************************************************/
    /*!
    @brief Recompiels the currently selected script/shader/
    */
    /**************************************************************************/
    void EditorTextEditor::Recompile()
    {
      DCTrace << "EditorTextEditor::Recompile: Recompiling... \n";
      if (CurrentShader) {
        DispatchSystemEvents::GraphicsCompileShaders();
      }
      if (CurrentScript) {
        Access().Creator.RebuildAllObjectsOnSpace();
        CompilationErrors.clear();
        DispatchSystemEvents::ScriptingCompile();
      }
    }

    /**************************************************************************/
    /*!
    @brief Saves the currently selected script/shader to a file.
    */
    /**************************************************************************/
    void EditorTextEditor::Save()
    {
      if (CurrentScript) {
        DCTrace << "EditorTextEditor::Save: '" << CurrentScript->Name() << "' \n";
        CurrentScript->Save(std::string(Text));        
        // Mark Zilch components for rebuilding on the next frame before we recompile them
        //DispatchSystemEvents::EditorRebuildZilchComponents();
        Access().Creator.RebuildAllObjectsOnSpace();
        DispatchSystemEvents::ScriptingCompile();
      }
      else if (CurrentShader) {
        DCTrace << "EditorTextEditor::Save: '" << CurrentShader->Name() << "' \n";
        CurrentShader->Save(std::string(Text), CurrentShaderType);
        DispatchSystemEvents::GraphicsCompileShaders();
      }    
    }

    void EditorTextEditor::Print()
    {
      DCTrace << " EditorTextEditor::Print: \n";
    }

    /**************************************************************************/
    /*!
    @brief Displays the current compilation errors in a small window.
    */
    /**************************************************************************/
    void EditorTextEditor::DisplayCompilationErrors()
    {
      if (CompilationErrors.empty())
        return;

      static bool opened = true;
      auto overlaySize = ImVec2(200, 100);
      ImGui::SetNextWindowPos(ImVec2(40,100), ImGuiSetCond_Once);
      if (!ImGui::Begin("Compilation Errors:", &opened, overlaySize, 0.5f, ImGuiWindowFlags_NoTitleBar))
      {
        ImGui::End();
        return;
      }

      ImGui::Columns(2, "compilationColumns");
      ImGui::TextColored(ImVec4(0, 1, 0, 1), "Script") ; ImGui::NextColumn();
      ImGui::SetColumnOffset(1, 120);
      ImGui::TextColored(ImVec4(1, 0, 0, 1), "Description"); ImGui::NextColumn();
      ImGui::Separator();
      for (auto& error : CompilationErrors) {
        if (ImGui::Selectable(error.ScriptName.c_str())) {}
        // If the user double clicks on the script, open it!
        if (GUI::IsMouseDoubleClicked()) {
          Load(ZilchScript::Find(error.ScriptName));
        }
        ImGui::NextColumn();

        ImGui::Text(error.Message.c_str()); ImGui::NextColumn();
        ImGui::Separator();
      }



      ImGui::End();

    }

    /**************************************************************************/
    /*!
    @brief Opens the currently selected script/shader on the specified
           source code text editor.
    */
    /**************************************************************************/
    void EditorTextEditor::OpenOnExternalEditor()
    {
      std::string qm = "\"";
      auto executablePath = boost::filesystem::initial_path().string();
      std::string filePath = executablePath + "\\";
      if (CurrentScript) {
        filePath += CurrentScript->getResourcePath();
      }
      if (CurrentShader) {
        DCTrace << "EditorTextEditor::OpenOnExternalEditor: Currently not working with shaders! \n";
        return;
      }

      // Fix me :(
      //std::string extTextEditorPath = executablePath + "\\Tools\\" + Access().Settings.ExternalTextEditor + ".exe.lnk";
      std::string notepadPath = "\\Notepad++\\notepad++.exe";
      std::string sublimePath = "\\Sublime\\sublime_text.exe";

      std::string editorPath;
      if (Access().Settings.ExternalTextEditor == "Notepad++")
        editorPath = notepadPath;
      else
        editorPath = sublimePath;            

      std::string extTextEditorPath = executablePath + "\\Tools" + editorPath;
      FileSystem::CorrectPath(extTextEditorPath);
      FileSystem::CorrectPath(filePath);

      // Get the arguments
      std::string command = qm + extTextEditorPath + qm; // +" " + filePath;
      std::vector<std::string> arguments;
      std::string fileArgument = qm + filePath + qm;
      arguments.push_back(fileArgument);

      FileSystem::Execute(command, arguments);
    }

    /**************************************************************************/
    /*!
    @brief Checks for specific inputs while the TextEditor window is open.
    */
    /**************************************************************************/
    void EditorTextEditor::CheckHotkeys()
    {
      auto io = ImGui::GetIO();

      // CTRL-Modified
      if (io.KeyCtrl) {
        // S
        bool s_keyPressed = io.KeysDown[io.KeyMap[ImGuiKey_S]];
        auto s_keyPressedDur = io.KeysDownDuration[io.KeyMap[ImGuiKey_S]];
        auto s_keyPressedPrevDur = io.KeysDownDurationPrev[io.KeyMap[ImGuiKey_S]];
        if (s_keyPressed && (s_keyPressedDur == s_keyPressedPrevDur)) {          
          DispatchSystemEvents::EditorSave();
        }
        // Tab
        bool tab_keyPressed = io.KeysDown[io.KeyMap[ImGuiKey_Tab]];
        auto tab_keyPressedDur = io.KeysDownDuration[io.KeyMap[ImGuiKey_Tab]];
        auto tab_keyPressedPrevDur = io.KeysDownDurationPrev[io.KeyMap[ImGuiKey_Tab]];
        if (tab_keyPressed && (tab_keyPressedDur == tab_keyPressedPrevDur)) {
          OpenOnExternalEditor();
        }
      }

    }

    /**************************************************************************/
    /*!
    @brief Clears the TextEditor's selected files.
    */
    /**************************************************************************/
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
    @brief Called when scripts are about to be compiled.
    */
    /**************************************************************************/
    void EditorTextEditor::OnScriptingLibraryAboutToCompile(Events::ScriptingLibraryAboutToCompile * event)
    {
      CompilationErrors.clear();
    }

    /**************************************************************************/
    /*!
    @brief Once scripts have been successfully recompiled into a new library,
           reload from file.
    */
    /**************************************************************************/
    void EditorTextEditor::OnScriptingLibraryCompiled(Events::ScriptingLibraryCompiled * event)
    {
      Reload();
    }

    /**************************************************************************/
    /*!
    @brief Once a script has failed to compile, display the message.. roughly!
    */
    /**************************************************************************/
    void EditorTextEditor::OnScriptingErrorMessageEvent(Events::ScriptingErrorMessage * event)
    {
      // Find the name of the script
      auto parse = Utils::String::Split(event->Message, " ");
      auto scriptName = FileSystem::FileNoExtension(parse[1]);

      //if (!CurrentScript)
      //  return;

      Windows::PopUpData data;
      data.Title = "'" + scriptName + "' has failed to compile!";
      data.Message = event->Message;
      data.Confirmation = "Back";
      auto popUp = WindowPtr(new Windows::PopUp(data));
      GUI::Add(popUp);

      // Add the compilation error to be tracked
      CompilationErrors.push_back(CompilationError(scriptName, event->Message));
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

    /**************************************************************************/
    /*!
    @brief If the window gained focus, and the Editor has been set to recompile
           scripts on it, compile and reload the script into the Editor.
    */
    /**************************************************************************/
    void EditorTextEditor::OnWindowGainedFocusEvent(Events::WindowGainedFocus * event)
    {
      if (Access().Settings.CompileOnContextSwitch) {
        Recompile();
        //Reload();
      }
    }

  }
}