/******************************************************************************/
/*!
@file   EditorTextEditor.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/28/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EditorModule.h"

#include "../../Resources/ZilchScript.h"
#include "../../Resources/Shader.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorTextEditor : public EditorModule {
      friend class Editor;
    public:
      enum class ExternalEditor {
        Notepadplusplus,
        Sublime
      };

      void Display();      
      void Load(ZilchScriptPtr script);
      void Load(Shader* shader, Shader::Type type);
      void Reload();
      void Save();
      EditorTextEditor(Editor& editor);
      
      

    private:      
      std::string Title;
      ZilchScriptPtr CurrentScript;
      Shader* CurrentShader;
      Shader::Type CurrentShaderType;
      unsigned CharLimit = 1024 * 16;

      void OpenOnExternalEditor();
      void CheckHotkeys();
      void Clear();
      void Close();
      // Events
      void OnEditorSaveEvent(Events::EditorSave* event);
      void OnScriptingErrorMessageEvent(Events::ScriptingErrorMessage* event);
      void OnGraphicsCompileShadersErrorEvent(Events::GraphicsCompileShadersError* event);
      void OnWindowGainedFocusEvent(Events::WindowGainedFocus* event);

    };

  }
}