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

#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"

#include "..\..\Objects\Resources\GLShader.h"

namespace DCEngine {
  namespace Systems {

    class GraphicsGL : public System {
    
    public:
      GraphicsGL();
      
      void Initialize();
      void Update(float dt);
      void Terminate();

    private:



    }; // GraphicsGL 

  } // Systems
} //DCEngine
