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

  class Engine;

  namespace Systems {
    class InputSFML;
  }
 

  class Keyboard : public Entity {
    friend class Engine;
    friend class Systems::InputSFML;
  public:

    bool KeyIsDown(Keys key);
    bool KeyIsUp(Keys key);

  private:

    /* List of keys */
    bool KeyDown_Escape = false;
    bool KeyDown_Space = false;
    
    bool KeyDown_Up = false;
    bool KeyDown_Down = false;
    bool KeyDown_Left = false;
    bool KeyDown_Right = false;
    
    bool KeyDown_W = false;
    bool KeyDown_S = false;
    bool KeyDown_A = false;
    bool KeyDown_D = false;
    bool KeyDown_Q = false;
    bool KeyDown_E = false;
    bool KeyDown_Z = false;
    bool KeyDown_X = false;

    bool KeyDown_F1 = false;
    bool KeyDown_F2 = false;
    bool KeyDown_F3 = false;
    bool KeyDown_F4 = false;


    Keyboard::Keyboard() : Entity(std::string("Keyboard")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << " Interface - Constructor \n";
    }

    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  };

  using KeyboardPtr = std::unique_ptr<Keyboard>;
}