#pragma once

namespace DCEngine {

  class GameObject;
  class Space;

  namespace Systems {

    class Editor;
    class EditorArchetypes {
    public:
      EditorArchetypes(Editor& editor);

    private:
      Editor& EditorRef;
      GameObject* CurrentArchetype;
      Space* ArchetypeSpace;

    };

  }
}