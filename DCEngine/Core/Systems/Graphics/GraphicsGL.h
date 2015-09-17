/******************************************************************************/
/*!
\file   GraphicsGL.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  The

*/
/******************************************************************************/
#pragma once
#include "..\System.h"

// Libraries
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"
// OpenGL Mathematics for doing transformations
#define GLM_FORCE_RADIANS // functions taking degrees is deprecated.
#include "..\..\..\Dependencies\include\GLM\glm.hpp"
#include "..\..\..\Dependencies\include\GLM\gtc\matrix_transform.hpp"
#include "..\..\..\Dependencies\include\GLM\gtc\type_ptr.hpp"
// Graphics resources
#include "..\..\Objects\Resources\Shader.h"

namespace DCEngine {
  namespace Systems {

    class GraphicsGL {
      friend class Graphics;
          
    public:
      GraphicsGL();
      
      void Initialize();
      void Update(float dt);
      void Terminate();

      void Render();

    private:



    }; // GraphicsGL 

  } // Systems
} //DCEngine
