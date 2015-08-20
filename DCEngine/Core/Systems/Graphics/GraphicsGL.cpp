/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The 

*/
/******************************************************************************/
#include "GraphicsGL.h"

#include "..\..\Debug\Debug.h"

// OpenGL tutorial. Change the header to execute a different tutorial.
//#include "..\..\..\Tutorials\GLTriangle.h"
//#include "..\..\..\Tutorials\OpenGL\GLShaderTutorial.h"
//#include "..\..\..\Tutorials\OpenGL\GLTextureTutorial.h"
#include "..\..\..\Tutorials\OpenGL\GLCameraTutorial.h"


namespace DCEngine {
  
  std::unique_ptr<Tutorial::GLTutorial> GLTutorial;

  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    GraphicsGL::GraphicsGL() : System(std::string("GraphicsGLSystem"), EnumeratedSystem::GraphicsGL) {
      
    }
    
    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    void GraphicsGL::Initialize() {
      trace << "GraphicsGL::Initialize \n";
      
      // Tutorial code
      //GLTutorial.reset(new Tutorial::GLCameraTutorial());
      //GLTutorial->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  Update function for the graphics system. Renders objects on screen.
    */
    /**************************************************************************/
    void GraphicsGL::Update(float dt) {
      trace << "GraphicsGL::Update \n";
      
      // Tutorial code
     //GLTutorial->Update();
    }

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {
      trace << "GraphicsGL::Terminate \n";
      // Tutorial code
      //GLTutorial->Terminate();

    }




  }


}

