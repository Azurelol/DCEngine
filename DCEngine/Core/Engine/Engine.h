/******************************************************************************/
/*!
\file   Engine.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   7/30/2015
\brief  

*/
/******************************************************************************/

#pragma once
#include <memory> // unique_ptr

namespace DCEngine {
  class Engine {
    public:

      Engine();
      void Initialize();
      
      void Update(float dt);
      void Loop();

      auto Stop() { _Active = false; } // Signals the engine to stop running
      void Terminate();

      auto Getdt() { return dt; }
     // auto GetEngine() { return std::shared_ptr<Engine> = this; }


    private:
      float dt;
      float _FrameRate = 60.0f;
      bool _Active;
  };


}