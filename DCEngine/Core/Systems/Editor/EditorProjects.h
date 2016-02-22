/******************************************************************************/
/*!
@file   EditorProjects.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   2/20/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "../../Resources/Archetype.h"
#include "../System.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorProjects : public Module {
    public:
      void WindowProjectProperties();
      void SaveProject();
      void ToggleProperties();

      EditorProjects(Editor& editor);
      ~EditorProjects();

    private:
      Editor& EditorRef;
      bool WindowProjectsPropertiesEnabled;
    };


  }
}