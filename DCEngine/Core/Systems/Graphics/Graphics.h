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
#include "../../Objects/Components/GraphicsSpace.h"

namespace DCEngine {
  namespace Systems {

    class Graphics : public System {
    public:
      Graphics();

      void Initialize();
      void Update(float dt);
      void Terminate();

      void Register(GraphicsSpace& graphicsSpace);

      void DrawSprite(GameObject& gameObj);
      void DrawModel(GameObject& gameObj);
      void DrawDebug();  

    private:
      // Graphics handler
      std::unique_ptr<GraphicsGL> GraphicsHandler;
      // The active camera
      const int screenwidth_ = 800;
      const int screenheight_ = 600;
      Mat4 ProjMatrix;
      Mat4 ViewMatrix;
      Mat4 ViewProjMatrix;

      // Container of graphics spaces accessing this system
      std::vector<GraphicsSpace> graphicsSpaces_;

    };


  }

}