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
#include "..\..\..\Tutorials\OpenGL\GLTextureTutorial.h"
#include "..\..\..\Tutorials\OpenGL\GLCameraTutorial.h"

std::unique_ptr<Tutorial::GLTutorial> GLTutorial;

namespace DCEngine {
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
      GLTutorial.reset(new Tutorial::GLCameraTutorial());
      GLTutorial->Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  Update function for the graphics system. Renders objects on screen.
    */
    /**************************************************************************/
    void GraphicsGL::Update(float dt) {
      std::cout << "GraphicsGL::Update" << std::endl;
      
      // Tutorial code
      GLTutorial->Update();
    }

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {

      // Tutorial code
      GLTutorial->Terminate();

    }




  }


}

