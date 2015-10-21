#pragma once
#include "../System.h"

// Library used:
#include <IMGUI\imgui.h>
//#include <SFML\Window.hpp> // Does thie need to be included?

namespace DCEngine {
  namespace Systems {

    //class sf::Window;

    class Editor;
    class GUI {
      friend class Editor;
    public:

    private:
      GUI();
      void Initialize();
      void Update(float dt);
      void Terminate();

      // imgui bindings
      //IMGUI_API bool ImGuiSFMLInitialize(sf::Window* windowContext, bool installCallbacks);
      //IMGUI_API void ImGuiSFMLTerminate();
      //IMGUI_API void ImGuiSFMLNewFrame();      
      //// If we want to reset the rednering device without losing the ImGui state
      //IMGUI_API void ImGuiSFMLInvalidateDeviceObjects();
      //IMGUI_API bool ImGuiSFMLCreateDeviceObjects();
      // Callbacks? How would imgui work with SFML?



    };
  }
}