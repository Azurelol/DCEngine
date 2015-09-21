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
#include "../../Components/GraphicsSpace.h"

namespace DCEngine {  
  class Engine;

  namespace Systems {

    class Graphics : public System {
      friend class Engine;
    public:

      void StartFrame();
      void EndFrame();
      void Register(GraphicsSpace& graphicsSpace);
      void DrawSprite(GameObject& gameObj);
      void DrawModel(GameObject& gameObj);
      void DrawDebug();  

    private:

      std::unique_ptr<GraphicsGL> GraphicsHandler;
      const int screenwidth_ = 800;
      const int screenheight_ = 600;
      Mat4 ProjMatrix;
      Mat4 ViewMatrix;
      Mat4 ViewProjMatrix;
      std::vector<GraphicsSpace*> graphicsSpaces_; //!< Container of graphics spaces accessing this system

      Graphics();
      void Initialize();
      void Update(float dt);
      void Terminate();

    };


  }

}