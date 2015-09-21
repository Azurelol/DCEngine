/******************************************************************************/
/*!
\file   Window.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  The interface for the window handler.

References:
GLFW implementation: "http://www.learnopengl.com/#!Getting-Started/Hello-Window"


*/
/******************************************************************************/
#include "Window.h"

#include "..\..\Debug\Debug.h"

// SFML implementation
#if(USE_SFML)
//#include "WindowSFML.h"
//std::unique_ptr<DCEngine::Systems::WindowSFML> WindowHandler;
// GLFW implementation
#else
//#include "WindowGLFW.h"
//std::unique_ptr<DCEngine::Systems::WindowGLFW> WindowHandler;
#endif

namespace DCEngine {
  namespace Systems {

    Window::Window() : System(std::string("WindowSystem"), EnumeratedSystem::Window) {
        #if(USE_SFML)
        trace << "*Using SFML Context for Window and Input \n";
        WindowHandler.reset(new WindowSFML());
        #else
        trace << "Window::Window() - Using GLFW Context \n";
        WindowHandler.reset(new WindowGLFW());
        #endif
    }

    void Window::Initialize() {
      if (TRACE_ON)
        trace << "Window::Initialize \n";

      WindowHandler->SetWindowSize(Width, Height);
      WindowHandler->SetWindowCaption(Caption);
      WindowHandler->Initialize();
      // Store a pointer to the window object. This will be used by the 'Input' system.
      //WindowObj.reset(WindowHandler->GetWindow());
      //WindowObj = WindowHandler->GetWindow();
    }

    void Window::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::Update \n";

      WindowHandler->Update(dt);
    }

    void Window::Terminate() {
      if (TRACE_ON)
        trace << "Window::Terminate \n";

      WindowHandler->Terminate();
    }

    void Window::StartFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::StartFrame \n";

      WindowHandler->StartFrame();
    }

    void Window::EndFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::EndFrame \n";

      WindowHandler->EndFrame();
    }

  }





}