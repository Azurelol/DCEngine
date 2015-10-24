/******************************************************************************/
/*!
\file   ImGuiSFML.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   10/23/2015
\brief  An SFML backend for ImGui.
*/
/******************************************************************************/
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
    
    struct OpenGLStateData {
      GLint lastProgram;
      GLint lastTexture;
      GLint lastArrayBuffer; 
      GLint lastElementArrayBuffer; 
      GLint lastVertexArray;
      GLint lastBlendSrc;
      GLint lastBlendDst;
      GLint lastBlendEquationRGB; 
      GLint lastBlendEquationAlpha; 
      GLboolean lastEnableBlend;
      GLboolean lastEnableCullFace;
      GLboolean lastEnableDepthTest;
      GLboolean lastEnableScissorTest;

    };

    class GUI;
    class ImGuiSFML {
      friend class GUI;
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
      ImGuiSFML();
      void Initialize();
      void StartFrame();
      void Render();
      void Terminate();

      /* imgui bindings */
      IMGUI_API bool ImGuiSFMLInitialize(sf::Window* windowContext, bool installCallbacks);
      IMGUI_API void ImGuiSFMLTerminate();
      
      /* Events*/
      IMGUI_API void ImGuiSFMLEventsUpdate();
      IMGUI_API void ImGuiSFMLBindEvents();
      IMGUI_API void ImGuiSFMLProcessEvent(sf::Event& event);
            
      /* Rendering */
      IMGUI_API bool ImGuiSFMLCreateDeviceObjects();
      IMGUI_API void ImGuiSFMLInitializeRendering();  

      // The function we pass to ImGui for rendering must be marked static?
      IMGUI_API static void ImGuiSFMLRenderDrawLists(ImDrawData* draw_data);
      IMGUI_API static OpenGLStateData ImGuiSFMLBackupGLState();
      IMGUI_API static void ImGuiSFMLRestoreState(OpenGLStateData& currentState);

      IMGUI_API void ImGuiSFMLGenerateFontTexture();
      //IMGUI_API static void ImGuiSFMLRenderDrawLists(ImDrawList** const cmdLists, int cmdListsCount);


    };
  }
}