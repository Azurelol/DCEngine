/******************************************************************************/
/*!
\file   Window.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/17/2015
\brief  The interface for the window handler.

References:
GLFW implementation: "http://www.learnopengl.com/#!Getting-Started/Hello-Window"

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Window.h"
#include "../../Engine/Engine.h"

namespace DCEngine {
  namespace Systems {
    
    /**************************************************************************/
    /*!
    @brief Constructor for the Window system.
    */
    /**************************************************************************/
    Window::Window(std::string& caption, unsigned int framerate, unsigned int width, 
                   unsigned int height, bool fullScreen) :
                      System(std::string("WindowSystem"), EnumeratedSystem::Window),
                      Caption(caption), Framerate(framerate), Width(width), Height(height),
                      Fullscreen(fullScreen) {

      DCTrace << "*Using SFML Context for Window and Input \n";
      WindowHandler.reset(new WindowSFML(*this));
    }

    /**************************************************************************/
    /*!
    @brief Initializes the Window system.
    */
    /**************************************************************************/
    void Window::Initialize() {
      if (TRACE_ON)
        DCTrace << "Window::Initialize \n";

      // Connect to events
      Subscribe();
      WindowHandler->Initialize();
    }

    /**************************************************************************/
    /*!
    @brief Subscribes to engine events.
    */
    /**************************************************************************/
    void Window::Subscribe()
    {
      Daisy->Connect<Events::FullscreenEnabledEvent>(&Window::OnFullscreenEnabledEvent, this);
      Daisy->Connect<Events::EngineExit>(&Window::OnEngineExitEvent, this);
      Daisy->Connect<Events::SetWindowCaption>(&Window::OnSetWindowCaptionEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Toggles fullscreen on.
    */
    /**************************************************************************/
    void Window::OnFullscreenEnabledEvent(Events::FullscreenEnabledEvent * event)
    {
      DCTrace << "Window::OnFullscreenEnabledEvent - \n";
      setFullscreen();
    }

    void Window::OnEngineExitEvent(Events::EngineExit * event)
    {
      DCTrace << "Window::OnEngineExitEvent - \n";
      WindowHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief Sets the current window's caption.
    */
    /**************************************************************************/
    void Window::OnSetWindowCaptionEvent(Events::SetWindowCaption * event)
    {
      Caption = event->Caption;
    }

    /**************************************************************************/
    /*!
    @brief Updates the Window system.
    @param dt The delta time.
    */
    /**************************************************************************/
    void Window::Update(float dt) {
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Window::Update \n";
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
        DCTrace << "Window::Terminate \n";

      WindowHandler->Terminate();
    }

    /**************************************************************************/
    /*!
    @brief Starts the current frame.
    */
    /**************************************************************************/
    void Window::StartFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Window::StartFrame \n";

      WindowHandler->StartFrame();
    }

    /**************************************************************************/
    /*!
    @brief Ends the current frame.
    */
    /**************************************************************************/
    void Window::EndFrame() {
      if (TRACE_ON && TRACE_UPDATE)
        DCTrace << "Window::EndFrame \n";

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