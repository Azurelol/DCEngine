#pragma once

namespace DCEngine {
  namespace Systems {

    class EditorWindow {
    public:
      virtual void Display() = 0;

    private:
      bool Enabled;


    };


  }
}