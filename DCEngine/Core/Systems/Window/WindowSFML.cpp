#include "WindowSFML.h"

#include "..\..\Engine\Engine.h"

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy;
  
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the WindowSFML class.
    */
    /**************************************************************************/
    WindowSFML::WindowSFML() {
    }

    WindowSFML::~WindowSFML() {
    }

    /**************************************************************************/
    /*!
    \brief  Initializes SFML, configuring the window before creating it.
    */
    /**************************************************************************/
    void WindowSFML::Initialize() {
      
      // Tells SFML the settings for the underlying OpenGL context
      sf::ContextSettings settings;
      settings.depthBits = _depthBits; 
      settings.stencilBits = _stencilBits; 
      settings.antialiasingLevel = _antiAliasingLevel; 
      settings.majorVersion = _majorVersion;
      settings.minorVersion = _minorVersion;
      
      // Create the window      
      _window.reset(new sf::Window(sf::VideoMode(_width, _height), _caption, sf::Style::Default, settings));
      //_window = new sf::Window(sf::VideoMode(_width, _height), _caption, sf::Style::Default, settings);
      // After this call, the application will run at the same frequency as the monitor's refresh rate
      _window->setVerticalSyncEnabled(true);
      // In some situations you want the application to run at a given framerate. (!) Do not mix with setVSync
      //_window->setFramerateLimit(60); 

      // Assert that the window was created
      if (_window == nullptr) {
        trace << "Failed to create SFNL window \n";
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
      if (_event.type == sf::Event::Closed)
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
      _window->display();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void WindowSFML::Terminate() {
      _window->close();
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
      _width = width;
      _height = height;
    }

    /**************************************************************************/
    /*!
    \brief  Sets the window's caption
    \param  The caption, in a string.
    */
    /**************************************************************************/
    void WindowSFML::SetWindowCaption(std::string caption) {
      _caption = caption;
    }

  }


  


}