#pragma once

namespace DCEngine {

  class EditorTool {
  public:
    virtual void Display() = 0;
  };

  using EditorToolPtr = std::shared_ptr<EditorTool>;

}