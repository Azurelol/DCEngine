/******************************************************************************/
/*!
\file   Window.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/1/2015
\brief  The abstract interface for the Window class.

*/
/******************************************************************************/
#include "../System.h"
#pragma once

/* Libraries */
#define GLEW_STATIC
#include "..\..\..\Dependencies\include\GLEW\glew.h"

#define USE_GLFW 1

namespace DCEngine {
  namespace Systems {
    class Window : public System {
    public:
      Window();
      void Initialize();
      void Update(float dt);
      void Terminate();

      void StartFrame();
      void EndFrame();

    private:
      const std::string Caption = "Daisy Chain Engine v.lol";
      const int Width = 1024;
      const int Height = 600;
    };
  }


}
