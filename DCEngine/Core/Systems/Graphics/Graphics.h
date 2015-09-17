/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  The graphics system is the bridge between the rendering calls from the 
        entity components and the underlying low-level OpenGL renderer. It
        receives rendering requests and draws on the window.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"

namespace DCEngine {
  namespace Systems {

    class Graphics : public System {
    public:
      Graphics();

      void Initialize();
      void Update(float dt);
      void Terminate();
      
      void Render();

    private:
      

    };


  }

}