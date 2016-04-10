/******************************************************************************/
/*!
@file   WindowsSFML.cpp
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/12/2015
@brief  Handling the window created by SFML functions.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h> // Oh dios mio

#include "WindowSFML.h"
#include "Window.h"
#include "..\..\Engine\Engine.h"
#include <sstream>


namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the WindowSFML class.
    */
    /**************************************************************************/
    WindowSFML::WindowSFML(Window& windowInterface) : WindowInterface(windowInterface) {
    }

    WindowSFML::~WindowSFML() {
    }

    sf::Vector2i WindowSFML::getWindowDimensions()
    {
      auto windowSize = WindowContext->getSize();
      return sf::Vector2i(windowSize.x, windowSize.y);
    }

    /**************************************************************************/
    /*!
    @brief  Switches back and forth between FullScreen and Windowed mode.
    @param  The mode of the window. FullScreen or Windowed.
    */
    /**************************************************************************/
    void WindowSFML::setFullScreen()
    {
      if (Mode != WindowMode::Fullscreen) {
        setWindow(WindowMode::Fullscreen);
        Mode = WindowMode::Fullscreen;
      }
      else {
        setWindow(WindowMode::Default);
        Mode = WindowMode::Default;
      }
    }

		void WindowSFML::resizeWindow(float x, float y)
		{
			WindowInterface.Settings.ScreenWidth = x;
			WindowInterface.Settings.ScreenHeight = y;
			if (Mode != WindowMode::Fullscreen)
			{
				WindowContext->setSize(sf::Vector2u(unsigned(x), unsigned(y)));
			}
			else
			{
				WindowContext->create(sf::VideoMode(
					WindowInterface.Settings.ScreenWidth, WindowInterface.Settings.ScreenHeight),
					WindowInterface.Caption, sf::Style::Fullscreen, ContextSettings);
				Daisy->getSystem<GUI>()->Initialize();
			}
		}

		void WindowSFML::recreateWindow(void)
		{
			ContextSettings.antialiasingLevel = WindowInterface.Settings.Samples;
			WindowContext->create(sf::VideoMode(
				WindowInterface.Settings.ScreenWidth, WindowInterface.Settings.ScreenHeight),
				WindowInterface.Caption, sf::Style::Titlebar | sf::Style::Close, ContextSettings);
		}

    /**************************************************************************/
    /*!
    @brief  Sets the window context.
    @param  The mode of the window. FullScreen or Windowed.
    @todo   Don't directly call the Graphics system by friending it. Instead
            send an event to replace it.
      2) Line 92, 93 reload VAO after creating a new window(fullscreen->window, window->fullscreen)
    */
    /**************************************************************************/
    void WindowSFML::setWindow(WindowMode style)
    {
      // Save the current OpenGL state
			//auto graphicsSystem = Daisy->getSystem<Graphics>();
			//Daisy->getSystem<GUI>()->Initialize();
      // This is stupid, but I can't pass in the sf::Style enum as a param :(
			ContextSettings.antialiasingLevel = WindowInterface.Settings.Samples;

      switch (style) {
      case WindowMode::Default:
        WindowInterface.Settings.ScreenWidth = widthRecord;
        WindowInterface.Settings.ScreenHeight = heightRecord;
        WindowContext->create(sf::VideoMode(
					WindowInterface.Settings.ScreenWidth, WindowInterface.Settings.ScreenHeight),
          WindowInterface.Caption, sf::Style::Titlebar | sf::Style::Close, ContextSettings);
        DispatchSystemEvents::WindowFullScreenDisabled();
        break;
      case WindowMode::Fullscreen:
        widthRecord = WindowInterface.Settings.ScreenWidth;
        heightRecord = WindowInterface.Settings.ScreenHeight;
        //WindowInterface.Settings.ScreenWidth = sf::VideoMode::getDesktopMode().width;
        //WindowInterface.Settings.ScreenHeight = sf::VideoMode::getDesktopMode().height;
        WindowContext->create(sf::VideoMode(
					WindowInterface.Settings.ScreenWidth, WindowInterface.Settings.ScreenHeight),
          WindowInterface.Caption, sf::Style::Fullscreen, ContextSettings);
        DispatchSystemEvents::WindowFullScreenEnabled();
        break;
      }
      // After this call, the application will run at the same frequency as the monitor's refresh rate
      //WindowContext->setVerticalSyncEnabled(true);
      // In some situations you want the application to run at a given framerate. (!) Do not mix with setVSync
      WindowContext->setFramerateLimit(WindowInterface.Settings.Framerate);

      // Restore the previous OpenGL state
      //Daisy->getSystem<Graphics>()->RestoreState();
      Daisy->getSystem<GUI>()->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  Initializes SFML, configuring the window before creating it.
    */
    /**************************************************************************/
    void WindowSFML::Initialize() {

      // Stores the settings for the underlying SFML window context
      ContextSettings.depthBits = _depthBits;
      ContextSettings.stencilBits = _stencilBits;
      ContextSettings.antialiasingLevel = WindowInterface.Settings.Samples;
      ContextSettings.majorVersion = _majorVersion;
      ContextSettings.minorVersion = _minorVersion;

      // If it starts as fullscreen
      if (WindowInterface.Settings.Fullscreen) {
        WindowContext.reset(new sf::Window(sf::VideoMode(sf::VideoMode::getDesktopMode().width,
          sf::VideoMode::getDesktopMode().height),
          WindowInterface.Caption, sf::Style::Fullscreen, ContextSettings));
        Mode = WindowMode::Fullscreen;
        DispatchSystemEvents::WindowFullScreenEnabled();
				WindowInterface.Settings.ScreenWidth = sf::VideoMode::getDesktopMode().width;
				WindowInterface.Settings.ScreenHeight = sf::VideoMode::getDesktopMode().height;
      }
      // Or if it starts as windowed
      else {
        WindowContext.reset(new sf::Window(sf::VideoMode(
					WindowInterface.Settings.ScreenWidth, WindowInterface.Settings.ScreenHeight),
          WindowInterface.Caption, sf::Style::Titlebar | sf::Style::Close, ContextSettings));
        Mode = WindowMode::Default;
        DispatchSystemEvents::WindowFullScreenDisabled();
      }

      WindowContext->setFramerateLimit(WindowInterface.Settings.Framerate);

      // Configures the window context, then creates it
      //setWindow(WindowMode::Default);

      // Assert that the window was created
      if (WindowContext == nullptr) {
        DCTrace << "Failed to create SFML window \n";
        // Terminate??
      }
    }

    /**************************************************************************/
    /*!
    \brief  Updates the window.
    */
    /**************************************************************************/
    void WindowSFML::Update(float dt) {

      // Checks at the start of loop iteration if SFML has been instructed
      // to close, and if so tell the engine to stop running.
      if (EventObj.type == sf::Event::Closed)
      {
        Terminate();
      }
    }

    /**************************************************************************/
    /*!
    \brief  Starts the current frame.
    */
    /**************************************************************************/
    void WindowSFML::StartFrame() {

    }

    /**************************************************************************/
    /*!
    \brief Displays on the screen what has been rendered to the window so far,
           typically called after all OpenGL rendering calls have been done
           for the current frame.
    */
    /**************************************************************************/
    void WindowSFML::EndFrame() {
      WindowContext->display();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void WindowSFML::Terminate() {
      WindowContext->close();
      Daisy->Stop();
    }
  }
}
