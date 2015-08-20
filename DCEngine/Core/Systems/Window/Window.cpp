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

// GLFW implementation
#if(USE_GLFW)
#include "WindowGLFW.h"
std::unique_ptr<DCEngine::Systems::WindowGLFW> GLFWHandler;
#endif

namespace DCEngine {
  namespace Systems {

    Window::Window() : System(std::string("WindowSystem"), EnumeratedSystem::Window) {
      GLFWHandler.reset(new WindowGLFW());
    }

    void Window::Initialize() {
      if (TRACE_ON)
        trace << "Window::Initialize \n";

      GLFWHandler->SetWindowSize(Width, Height);
      GLFWHandler->SetWindowCaption(Caption);
      GLFWHandler->Initialize();
      // Store a pointer to the window object. This will be used by the 'Input' system.
      //WindowObj.reset(GLFWHandler);
    }

    void Window::Update(float dt) {
      if (TRACE_ON)
        trace << "Window::Update \n";

      GLFWHandler->Update(dt);
    }

    void Window::Terminate() {
      if (TRACE_ON)
        trace << "Window::Terminate \n";

      GLFWHandler->Terminate();
    }

    void Window::StartFrame() {
      if (TRACE_ON)
        trace << "Window::StartFrame \n";

      GLFWHandler->StartFrame();
    }

    void Window::EndFrame() {
      if (TRACE_ON)
        trace << "Window::EndFrame \n";

      GLFWHandler->EndFrame();
    }

  }





}