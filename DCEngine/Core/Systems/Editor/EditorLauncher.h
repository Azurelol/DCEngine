/******************************************************************************/
/*!
@file   EditorLauncher.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/10/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "EditorModule.h"

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorLauncher : public EditorModule {
    public:
      EditorLauncher();
      ~EditorLauncher();
      void Launch();
      void Close();
      void SetEditorCamera(bool);
      void CreateEditorCamera();
      void RemoveEditorCamera();
      void DisplaySplashScreen();

    private:
      bool SplashScreenEnabled;


    };

  }
}