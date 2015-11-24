/******************************************************************************/
/*!
\file   Mouse.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The mouse input inteface for the engine.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "../Entity.h"

namespace DCEngine {

  class Engine;

  namespace Systems {
    class InputSFML;
  }
  
  class Mouse : public Entity {
    friend class Engine;
    friend class Systems::InputSFML;

  public:

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(Mouse, Entity);
    #endif
    
    Mouse::Mouse() : Entity(std::string("Mouse")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << " Interface - Constructor \n";
    }

    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);

  private:

    bool MouseDown_Left = false;
    bool MouseDown_Right = false;
    bool MouseDown_Middle = false;

    void Initialize();
  };

  using MousePtr = std::unique_ptr<Mouse>;
}