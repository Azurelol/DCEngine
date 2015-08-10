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

#include "Tutorials\GLTriangle.h"


// std::unique_ptr<Tutorial::GLTutorial> GLTUTORIAL;

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
      std::cout << "GraphicsGL::Initialize" << std::endl;
      
      // Tutorial Code
      Tutorial::GLTriangle::Initialize();
    }

    /**************************************************************************/
    /*!
    \brief  The constructor for the Engine object.
    */
    /**************************************************************************/
    void GraphicsGL::Update(float dt) {
      std::cout << "GraphicsGL::Update" << std::endl;
      // Tutorial code
      Tutorial::GLTriangle::Update();
    }

    /**************************************************************************/
    /*!
    \brief  
    */
    /**************************************************************************/
    void GraphicsGL::Terminate() {
    }




  }


}

