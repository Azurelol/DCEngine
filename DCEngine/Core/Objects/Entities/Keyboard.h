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
        trace << ObjName << " Interface - Constructor \n";
    }

    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
  };

  using KeyboardPtr = std::unique_ptr<Keyboard>;
}