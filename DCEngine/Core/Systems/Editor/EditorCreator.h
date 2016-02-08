#pragma once

namespace DCEngine {
  namespace Systems {

    class Editor;
    class EditorCreator {
    public:
      EditorCreator(Editor& editor);

    private:
      Editor& EditorRef;
      void CreateLight();

    };


  }
}
