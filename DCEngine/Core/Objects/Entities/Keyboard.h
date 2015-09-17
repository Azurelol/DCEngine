/******************************************************************************/
/*!
\file   Keyboard.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The keyboard input inteface for the engine.
*/
/******************************************************************************/
#pragma once
#include "../Entity.h"

namespace DCEngine {

  class Keyboard : public Entity {
  public:
    Keyboard::Keyboard() : Entity(std::string("Keyboard")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        trace << _name << " Interface - Constructor \n";
    }

    void Initialize();
  };

  using KeyboardPtr = std::unique_ptr<Keyboard>;
}