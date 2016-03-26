/******************************************************************************/
/*!
@file   EditorModule.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/22/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

#include "../System.h" 

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorModule : public Module {
    public:
      bool WindowEnabled;
      void Toggle(bool);
      void Toggle();
      Editor& Access();
      virtual void Display() {};
      virtual void Update() {};
      EditorModule(bool add);
      ~EditorModule();
    protected:
    private:
      void AddWindow();
      void OnEditorInitializeEvent(Events::EditorInitialize* event);
      void OnEditorUpdateEvent(Events::EditorUpdate* event);

      Editor* EditorRef;
      bool WillAddWindow;
    };

    using EditorModulePtr = EditorModule*;

  }
}