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
//#include "../System.h"
#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorProjects : public EditorModule {
    public:
      void WindowProjectProperties();
      void SaveProject();
      void ToggleProperties();

      void Display();
      EditorProjects(Editor& editor);
      ~EditorProjects();

    protected:
      

    private:
      bool WindowProjectsPropertiesEnabled;
      void Update();
    };


  }
}