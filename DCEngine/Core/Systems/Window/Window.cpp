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
    Window::Window(GraphicsConfig& config, const std::string& caption) :
                      System(std::string("WindowSystem"), EnumeratedSystem::Window),
                      Settings(config), Caption(caption) {

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
      Daisy->Connect<Events::WindowFullScreenToggle>(&Window::OnWindowFullScreenToggleEvent, this);
      Daisy->Connect<Events::EngineExit>(&Window::OnEngineExitEvent, this);
      Daisy->Connect<Events::SetWindowCaption>(&Window::OnSetWindowCaptionEvent, this);
			Daisy->Connect<Events::WindowResize>(&Window::OnWindowResizeEvent, this);
			Daisy->Connect<Events::WindowRecreate>(&Window::OnWindowRecreateEvent, this);
    }

    /**************************************************************************/
    /*!
    @brief Toggles fullscreen on.
    */
    /**************************************************************************/
    void Window::OnWindowFullScreenToggleEvent(Events::WindowFullScreenToggle * event)
    {
      DCTrace << "Window::OnWindowFullScreenToggleEvent - \n";
      setFullscreen();
    }

		void Window::OnWindowResizeEvent(Events::WindowResize * event)
		{
			DCTrace << "Window::OnWindowResizeEvent - \n";
			WindowHandler->resizeWindow(event->Dimensions.x, event->Dimensions.y);
		}

		void Window::OnWindowRecreateEvent(Events::WindowRecreate * event)
		{
			DCTrace << "Window::OnWindowResizeEvent - \n";
			WindowHandler->recreateWindow();
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
      Settings.ScreenWidth = WindowHandler->getWindowDimensions().x;
      Settings.ScreenHeight = WindowHandler->getWindowDimensions().y;
      CalculateFPS(dt);
      WindowHandler->Update(dt);
    }

    /**************************************************************************/
    /*!
    @brief Returns the engine's current framerate.
    @return The framerate.
    */
    /**************************************************************************/
    float Window::FPS()
    {
      return CurrentFramerate;
    }

    /**************************************************************************/
    /*!
    @brief Calculates the engine's current FPS.
    @param dt The delta time.
    */
    /**************************************************************************/
    float localCounter = 0;
    int frameCounter = 0;
    void Window::CalculateFPS(float dt)
    {
      localCounter += dt;
      ++frameCounter;
      if (localCounter > 0.5)
      {
        CurrentFramerate = static_cast<int>(frameCounter / localCounter);
        std::stringstream ss;
        ss << Caption << "              [FPS =" << CurrentFramerate << "]";
        WindowHandler->WindowContext->setTitle(ss.str());
        //windowsTitle = ss.str();
        localCounter = 0;
        frameCounter = 0;
      }
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
      return glm::vec2(Settings.ScreenWidth, Settings.ScreenHeight);
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