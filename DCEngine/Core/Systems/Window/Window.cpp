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
      trace << "Window::Initialize \n";
      GLFWHandler->SetWindowSize(Width, Height);
      GLFWHandler->SetWindowCaption(Caption);
      GLFWHandler->Initialize();      
    }

    void Window::Update(float dt) {
      trace << "Window::Update \n \n";
      GLFWHandler->Update(dt);
    }

    void Window::Terminate() {
      trace << "Window::Terminate \n";
      GLFWHandler->Terminate();
    }

    void Window::StartFrame() {
      trace << "Window::StartFrame \n";
      GLFWHandler->StartFrame();
    }

    void Window::EndFrame() {
      trace << "Window::EndFrame \n";
      GLFWHandler->EndFrame();
    }

  }





}