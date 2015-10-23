#pragma once
#include "../System.h"

// Library used:
#include <IMGUI\imgui.h>
#include <SFML\Window.hpp>

#include "../../Engine/Types.h"
//#include <SFML\Window.hpp> // Does thie need to be included?

namespace DCEngine {
  namespace Systems {

    class Editor;
    class GUI {
      friend class Editor;
    public:

    private:
      
      /* Data */
      sf::Window* WindowContext; //!< A pointer to the window context.
      sf::Clock TimeElapsed; //!< The time elapsed
      bool MousePressed[3] = { false, false, false };
      float MouseWheel = 0.0f;
      GLuint FontTexture = 0;

      /* System Functions */
      GUI();
      void Initialize();
      void Update(float dt);
      void Terminate();

      /* imgui bindings */
      IMGUI_API bool ImGuiSFMLInitialize(sf::Window* windowContext, bool installCallbacks);
      IMGUI_API void ImGuiSFMLTerminate();
      IMGUI_API void ImGuiSFMLNewFrame();      

      IMGUI_API void ImGuiSFMLBindEvents();
      IMGUI_API void ImGuiSFMLProcessEvent(sf::Event& event);
      
      IMGUI_API void ImGuiSFMLInitializeRendering();
      
      /* If we want to reset the rednering device without losing the ImGui state */
      IMGUI_API void ImGuiSFMLInvalidateDeviceObjects();
      IMGUI_API bool ImGuiSFMLCreateDeviceObjects();
      //Callbacks? How would imgui work with SFML?
      IMGUI_API void ImGuiSFMLInitializeEvents();
      IMGUI_API void ImGuiSFMLRenderDrawLists(ImDrawList** const cmdLists, int cmdListsCount);


    };
  }
}