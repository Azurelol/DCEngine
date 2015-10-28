/******************************************************************************/
/*!
@file   Input.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/17/2015
@brief  The abstract interface for the Input class.
@todo   Find another way to pass the event object to the GUI?

*/
/******************************************************************************/
#include "..\System.h"
#pragma once

#define USE_SFML 1

#if(USE_SFML)
#include "InputSFML.h"
#endif

namespace DCEngine{
  class Engine;

  namespace Systems {
    class GUI;
    class ImGuiSFML; // 
    class Input : public System {
      friend class Engine;
      friend class GUI;
      friend class ImGuiSFML;
    public:

    private:
      Input();
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

      std::unique_ptr<DCEngine::Systems::InputSFML> InputHandler;

    };

  }

}
