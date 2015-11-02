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
#include "../../Engine/Engine.h"
//#include "..\..\Debug\Debug.h"


// SFML implementation
#if(USE_SFML)
//#include "WindowSFML.h"
//std::unique_ptr<DCEngine::Systems::WindowSFML> WindowHandler;
// GLFW implementation
#endif

namespace DCEngine {
  namespace Systems {
    
    void Window::Serialize(Json::Value & root)
    {
    }

    void Window::Deserialize(Json::Value & root)
    {
    }

    /**************************************************************************/
    /*!
    @brief Constructor for the Window system.
    */
    /**************************************************************************/
    Window::Window(std::string& caption, unsigned int framerate, unsigned int width, unsigned int height) :
                      System(std::string("WindowSystem"), EnumeratedSystem::Window),
                      Caption(caption), Framerate(framerate), Width(width), Height(height) {

      trace << "*Using SFML Context for Window and Input \n";
      WindowHandler.reset(new WindowSFML(*this));
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Window system.
    */
    /**************************************************************************/
    void Window::Initialize() {
      if (TRACE_ON)
        trace << "Window::Initialize \n";

      // Grab the Window's settings from the Engine's configuration file:
      //WindowHandler->SetWindowSize(Width, Height);
      //WindowHandler->SetWindowCaption(Caption);
      WindowHandler->Initialize();
      // Store a pointer to the window object. This will be used by the 'Input' system.
      //WindowObj.reset(WindowHandler->GetWindow());
      //WindowObj = WindowHandler->GetWindow();
    }

    /**************************************************************************/
    /*!
    @brief Updates the Window system.
    @param dt The delta time.
    */
    /**************************************************************************/
    void Window::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::Update \n";
      // Update the current width's, heig
      Width = WindowHandler->getWindowDimensions().x;
      Height = WindowHandler->getWindowDimensions().y;      
      WindowHandler->Update(dt);
    }

    /**************************************************************************/
    /*!
    @brief Terminates the Window system.
    */
    /**************************************************************************/
    void Window::Terminate() {
      if (TRACE_ON)
        trace << "Window::Terminate \n";

      WindowHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief Starts the current frame.
    */
    /**************************************************************************/
    void Window::StartFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::StartFrame \n";

      WindowHandler->StartFrame();
    }

    /**************************************************************************/
    /*!
    @brief Ends the current frame.
    */
    /**************************************************************************/
    void Window::EndFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        trace << "Window::EndFrame \n";

      WindowHandler->EndFrame();
    }

    /**************************************************************************/
    /*!
    @brief Returns the current window context's dimensions.
    @return A 2d vector.
    */
    /**************************************************************************/
    glm::vec2 Window::getWindowDimensions()
    {
      return glm::vec2(Width, Height);
    }

    /**************************************************************************/
    /*!
    @brief Switches the current window to fullscreen.
    */
    /**************************************************************************/
    void Window::setFullscreen()
    {
      WindowHandler->setFullScreen();
    }

  }





}