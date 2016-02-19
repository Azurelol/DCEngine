#pragma once

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorCreator {
    public:
      EditorCreator(Editor& editor);
      void CreateLight();

    private:
      Editor& EditorRef;

    };


  }
}
