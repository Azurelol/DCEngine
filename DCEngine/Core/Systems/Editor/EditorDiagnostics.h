/*****************************************************************************/
/*!
\file   EditorDiagnostics.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   3/3/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "EditorModule.h"
#include "../../Engine/Timer.h"

namespace DCEngine {
  namespace Systems {
    class EditorDiagnostics : public EditorModule {
    public:

      void Display();
      EditorDiagnostics(Editor& editor);
      ~EditorDiagnostics();


    private:
      void Graphics();
      void Physics();
      void Events();
      void Actions();
      void GameObjects();

      void DisplayEvents(const std::string& name, EventDelegatesInfo& info);
      void DisplaySystemsHistogram(std::string, DCEngine::Time::FunctionTimeSliceVec&, bool calls = false);
      void Update();

    };
  }
}
