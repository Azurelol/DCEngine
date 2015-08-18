/******************************************************************************/
/*!
\file   WindowGLFW.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  

Functions include:
- Name
Description here.

References:
http://www.learnopengl.com/#!Getting-Started/Hello-Window

*/
/******************************************************************************/
#include "WindowGLFW.h"

#include "..\..\Engine\Engine.h"
#include "..\Input\InputGLFW.h"
#include "..\..\Debug\Debug.h"


namespace DCEngine {

  extern std::unique_ptr<Engine> ENGINE;

  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the WindowGLFW class.
    */
    /**************************************************************************/
    WindowGLFW::WindowGLFW() : System(std::string("WindowGLFWSystem"), EnumeratedSystem::WindowGLFW) {
    }
    

    //void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

    /**************************************************************************/
    /*!
    \brief  Initializes GLFW, configuring the window before creating it.
    */
    /**************************************************************************/
    void WindowGLFW::Initialize() {
      
      glfwInit();
      trace << "WindowGLFW::Initialize \n";

      // Tells GLFW which OpenGL version we want to use
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      // Tells GLFW explicitly that we want to use core-profile. This will result in
      // invalid operation errors when trying to call OpenGL's legacy functions
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      // Tells GLFW that the window should not be resizable by the user
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

      /* The glfwCreateWindow function requires the window width and height as its 
      first two arguments respectively. The third argument allows us to create a 
      name for the window; We can ignore the last 2 parameters. The function 
      returns a GLFWwindow object that we'll later need for other GLFW operations.  */
      _window = glfwCreateWindow(800, 600, "Daisy Chain Engine",
        nullptr, nullptr);
      
      // ASSERT
      if (_window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
      }

      /*  After that we tell GLFW to make the context of our window the main
      context on the current thread. */
      glfwMakeContextCurrent(_window);

      // GLEW manages function pointers for OpenGL, so we want to initialize
      // it before calling any OpenGL functions. Setting glewExperimental to
      // true uses more modern techniques for managing OpenGL functionality.
      glewExperimental = GL_TRUE;
      
      // ASSERT
      if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
      }

      // Callback functions
      glfwSetKeyCallback(_window, KeyCallback);

    }

    /**************************************************************************/
    /*!
    \brief  Updates the viewport, then polls window/input events.
    */
    /**************************************************************************/
    void WindowGLFW::Update(float dt) {
      trace << "WindowGLFW::Update \n";
      ViewportUpdate();
      PollEvents();
    }

    /**************************************************************************/
    /*!
    \brief  As soon as the game loop is exited, cleans/deletes all allocated
            resources.
    */
    /**************************************************************************/
    void WindowGLFW::Terminate() {
      glfwTerminate();
    }

    /**************************************************************************/
    /*!
    \brief  At the start of each render iteration clears the screen.
    */
    /**************************************************************************/
    void WindowGLFW::StartFrame() {
      // GLFW automatically creates a depth buffer for you
      glEnable(GL_DEPTH_TEST);
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State-setting function
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // State-using function
      trace << "WindowGLFW::StartFrame \n";
    }

    /**************************************************************************/
    /*!
    \brief  Ends the current frame.
    */
    /**************************************************************************/
    void WindowGLFW::EndFrame() {
      // Swaps the color buffer (A large color buffer that contains
      // color values for each pixel in GLFW's window) that has been used
      // to draw in during this iteration and show it as output to the screen.
      glfwSwapBuffers(_window);
      trace << "WindowGLFW::EndFrame \n";
    }

    /**************************************************************************/
    /*!
    \brief  Tells OpenGL the size of the rendering window
            First two parameters set the location of the lower left corner of the
            window. The third and fourth parameters set the width and height
            of the rendering window, which is the same as the GLFW window.
            
            Behind the scenes OpenGL uses data specified via glViewport to transform
            the 2D coordinates it processed to coordinates on your screen.
    */
    /**************************************************************************/
    void WindowGLFW::ViewportUpdate() {
      glViewport(0, 0, _width, _height);
      std::cout << "WindowGLFW::ViewportUpdate - Width: " << _width
                                           << " Height: " << _height << std::endl;
    }

    /**************************************************************************/
    /*!
    \brief  Polls events.
    */
    /**************************************************************************/
    void WindowGLFW::PollEvents() {
      
      // Checks at the start of loop iteration if GLFW has been instructed
      // to close, and if so the function returns true and the game loop
      // starts running.
      if (glfwWindowShouldClose(_window)) {
        ENGINE->Stop();
      }
        
      // Checks if any events are triggered (like keyboard input, or mouse 
      // movement events) and calls the corresponding functions (which we c
      // we set via callback methods). We call eventprocessing functions
      // at the start of a loop operation)
      glfwPollEvents();
    } 


  } // Systems
} // DCEngine
