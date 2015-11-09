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

  class Engine;

  namespace Systems {
    class InputSFML;
  }
  
  class Mouse : public Entity {
    friend class Engine;
    friend class Systems::InputSFML;

  public:
    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);

  private:

    bool MouseDown_Left = false;
    bool MouseDown_Right = false;
    bool MouseDown_Middle = false;


    Mouse::Mouse() : Entity(std::string("Mouse")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << " Interface - Constructor \n";      
    }

    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);
  };

  using MousePtr = std::unique_ptr<Mouse>;
}