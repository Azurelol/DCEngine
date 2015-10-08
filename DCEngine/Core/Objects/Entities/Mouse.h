/******************************************************************************/
/*!
\file   Mouse.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The mouse input inteface for the engine.
*/
/******************************************************************************/
#pragma once
#include "../Entity.h"

namespace DCEngine {

  class Mouse : public Entity {
  public:
    Mouse::Mouse() : Entity(std::string("Mouse")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        trace << ObjName << " Interface - Constructor \n";      
    }

    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
  };

  using MousePtr = std::unique_ptr<Mouse>;
}