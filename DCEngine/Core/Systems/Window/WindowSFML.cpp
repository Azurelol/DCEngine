#include "WindowSFML.h"
#include "Window.h"
#include "..\..\Engine\Engine.h"

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

    /**************************************************************************/
    /*!
    @brief  Sets the window context.
    @param  The mode of the window. FullScreen or Windowed.
    @todo   Don't directly call the Graphics system by friending it. Instead
            send an event to replace it.
    */
    /**************************************************************************/
    void WindowSFML::setWindow(WindowMode style)
    {
      // Create the window
      //if (WindowContext != nullptr)
      //  WindowContext->close();

      // Save the current OpenGL state
      Daisy->getSystem<Graphics>()->BackupState();

      // This is stupid, but I can't pass in the sf::Style enum as a param :(
      switch (style) {
      case WindowMode::Default:
        WindowContext->create(sf::VideoMode(WindowInterface.DefaultWidth, WindowInterface.DefaultHeight),
                              CaptionText, sf::Style::Default, ContextSettings);
        break;
      case WindowMode::Fullscreen :
        WindowContext->create(sf::VideoMode(WindowInterface.Width, WindowInterface.Height),
          CaptionText, sf::Style::Fullscreen, ContextSettings);        
        break;
      }
      // After this call, the application will run at the same frequency as the monitor's refresh rate
      //WindowContext->setVerticalSyncEnabled(true);
      // In some situations you want the application to run at a given framerate. (!) Do not mix with setVSync
      WindowContext->setFramerateLimit(60);

      // Restore the previous OpenGL state
      Daisy->getSystem<Graphics>()->RestoreState();

      // Reload textures
      //Daisy->getSystem<Content>()->LoadTextures();

      // The window context needs to give the input system a reference to this pointer
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
      ContextSettings.antialiasingLevel = _antiAliasingLevel;
      ContextSettings.majorVersion = _majorVersion;
      ContextSettings.minorVersion = _minorVersion;

      WindowContext.reset(new sf::Window(sf::VideoMode(WindowInterface.Width, WindowInterface.Height),
        CaptionText, sf::Style::Default, ContextSettings));
      WindowContext->setFramerateLimit(60);

      

      // Configures the window context, then creates it
      //setWindow(WindowMode::Default);

      // Assert that the window was created
      if (WindowContext == nullptr) {
        trace << "Failed to create SFML window \n";
        // Terminate??
      }
    }

    /**************************************************************************/
    /*!
    \brief  Updates the window.
    */
    /**************************************************************************/
    void WindowSFML::Update(float dt) {

      //auto currentTime = Clock.restart().asSeconds();
      //float fps = 1.f / (currentTime - LastTime);
      //LastTime = currentTime;
      //trace << "WindowSFML::Update - FPS: " << fps << "\n";

      // Checks at the start of loop iteration if SFML has been instructed
      // to close, and if so tell the engine to stop running.
      if (EventObj.type == sf::Event::Closed)
        Terminate();
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
  
    /**************************************************************************/
    /*!
    \brief  Sets the window size.
    \param  The width of the window.
    \param  The height of the window.
    */
    /**************************************************************************/
    void WindowSFML::SetWindowSize(int width, int height) {
      WindowInterface.Width = width;
      WindowInterface.Height = height;
    }

    /**************************************************************************/
    /*!
    \brief  Sets the window's caption
    \param  The caption, in a string.
    */
    /**************************************************************************/
    void WindowSFML::SetWindowCaption(std::string caption) {
      CaptionText = caption;
    }

  }


  


}