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
      void DisplaySystemsHistogram(std::string, DCEngine::Time::FunctionTimeSliceVec&, bool calls = false);
      void Update();

    };
  }
}
