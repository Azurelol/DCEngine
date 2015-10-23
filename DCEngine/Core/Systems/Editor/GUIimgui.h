#pragma once
#include "../System.h"

// Library used:
#include <IMGUI\imgui.h>
#include <SFML\Window.hpp>
#include <SFML\Graphics\Texture.hpp>

#include "../../Engine/Types.h"
//#include <SFML\Window.hpp> // Does thie need to be included?

namespace DCEngine {
  namespace Systems {
    
    class Editor;
    class GUI {
      friend class Editor;
    public:

    private:      
      /* Event Data */
      sf::Window* WindowContext; //!< A pointer to the window context.
      sf::Clock TimeElapsed; //!< The time elapsed
      bool MousePressed[2] = { false, false };
      float MouseWheel = 0.0f;      
      /* Rendering Data */
      sf::Texture FontTexture;


      /* System Functions */
      GUI();
      void Initialize();
      void Update(float dt);
      void Terminate();

      /* imgui bindings */
      IMGUI_API bool ImGuiSFMLInitialize(sf::Window* windowContext, bool installCallbacks);
      IMGUI_API void ImGuiSFMLTerminate();
      IMGUI_API void ImGuiSFMLNewFrame();      

      /* Events*/
      IMGUI_API void ImGuiSFMLEventsUpdate();
      IMGUI_API void ImGuiSFMLBindEvents();
      IMGUI_API void ImGuiSFMLProcessEvent(sf::Event& event);
            
      /* Rendering */
      IMGUI_API void ImGuiSFMLInitializeRendering();      
      IMGUI_API void ImGuiSFMLInvalidateDeviceObjects();
      IMGUI_API bool ImGuiSFMLCreateDeviceObjects();

      // The function we pass to ImGui for rendering must be marked static?
      IMGUI_API static void ImGuiSFMLRenderDrawLists(ImDrawData* draw_data);
      IMGUI_API static GLint ImGuiSFMLRenderStateSetup();
      IMGUI_API static void ImGuiSFMLRestoreState(GLint lastTexture);

      IMGUI_API void ImGuiSFMLGenerateFontTexture();
      //IMGUI_API static void ImGuiSFMLRenderDrawLists(ImDrawList** const cmdLists, int cmdListsCount);


    };
  }
}