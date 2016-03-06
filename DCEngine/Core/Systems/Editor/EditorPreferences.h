/******************************************************************************/
/*!
@file   EditorPreferences.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/4/2015
@brief  This file includes the implementation for the Editor's library widget.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorPreferences : public EditorModule {
    public:

      void Display();
      EditorPreferences(Editor& editor);
      ~EditorPreferences();

    protected:
    private:
      enum class Tab {
        Editor,
        Graphics,
        Audio,
        Debug,
        Style,
        StyleSizes,
        StyleColors,
      };

      void Update();
      void DisplayColumns();
      
      Tab CurrentTab;
      void DisplayTab();
      void TabEditor();
      void TabDebug();
      void TabStyle();
      void TabStyleSizes();
      void TabStyleColors();
      void TabGraphics();
      void TabAudio();

      

      void Save();


    };

  }
}