/******************************************************************************/
/*!
\file   WindowGLFW.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief
The x

Functions include:
- Name
Description here.

References:
http://www.learnopengl.com/#!Getting-Started/Hello-Window

*/
/******************************************************************************/
#include "WindowGLFW.h"

#include "..\..\Debug\Debug.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    WindowGLFW::WindowGLFW() : System(std::string("WindowGLFWSystem"), ES_WindowGLFW) {
    }
    

    /**************************************************************************/
    /*!
    \brief
    */
    /**************************************************************************/
    void WindowGLFW::Initialize() {
      
      glfwInit();
      Debug::PrintString("WindowGLFW::Initialize");

      /*  The first argument of glfwWindowHint tells us what option we want to configure,
      where we can select the option from a large enum of possible options prefixed with
      GLFW_. The second argument is an integer that sets the value of our option */

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
        Debug::PrintString("Failed to create GLFW window");
        glfwTerminate();
        // return -1;
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
        Debug::PrintString("Failed to initialize GLEW");
        // return -1
      }

    }

    /**************************************************************************/
    /*!
    \brief
    */
    /**************************************************************************/
    void WindowGLFW::Update(float dt) {
      Debug::PrintString("WindowGLFW::Update");
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
    }

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void WindowGLFW::PollEvents() {
      
      // Checks at the start of loop iteration if GLFW has been instructed
      // to close, and if so the function returns true and the game loop
      // starts running.
      while (!glfwWindowShouldClose(_window)) {

        // Checks if any events are triggered (like keyboard input, or mouse 
        // movement events) and calls the corresponding functions (which we can
        // we set via callback methods). We call eventprocessing functions
        // at the start of a loop operation)
        glfwPollEvents();
        
        PollWindowEvent();
      }      
    }



    /**************************************************************************/
    /*!
    \brief
    */
    /**************************************************************************/
    void WindowGLFW::PollWindowEvent() {
      // Swaps the color buffer (A large color buffer that contains
      // color values for each pixel in GLFW's window) that has been used
      // to draw in during this iteration and show it as output to the screen.
      glfwSwapBuffers(_window);

    }

  } // Systems
} // DCEngine
