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
      EditorModule(Editor& editor);
      ~EditorModule();

    private:
      void OnEditorUpdateEvent(Events::EditorUpdate* event);
      virtual void Update() = 0;

      Editor& EditorRef;
    };

  }
}