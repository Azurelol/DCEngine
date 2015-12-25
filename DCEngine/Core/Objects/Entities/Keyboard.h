/******************************************************************************/
/*!
\file   Keyboard.h
\author Christian Sagel, Connor Tilley
\par    email: c.sagel\@digipen.edu
\date   8/22/2015
\brief  The keyboard input inteface for the engine.
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

    class Keyboard : public Entity {
    //class Keyboard : public System {
    friend class Engine;
    friend class Systems::InputSFML;
  public:

    //#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    //ZilchDeclareDerivedType(Keyboard, Entity);
    //#endif

    //Keyboard::Keyboard() : System(std::string("Keyboard"), EnumeratedSystem::Keyboard) {
    Keyboard::Keyboard() : Entity(std::string("Keyboard")) {
      if (TRACE_ON && TRACE_CONSTRUCTOR)
        DCTrace << ObjectName << " Interface - Constructor \n";
    }

    bool KeyIsDown(Keys key);
    bool KeyIsUp(Keys key);

  private:

    /* List of keys */
    bool KeyDown_Escape = false;
    bool KeyDown_Space = false;
    bool KeyDown_LControl = false;
    bool KeyDown_RControl = false;
    bool KeyDown_LShift = false;
    bool KeyDown_RShift = false;

    bool KeyDown_Up = false;
    bool KeyDown_Down = false;
    bool KeyDown_Left = false;
    bool KeyDown_Right = false;

    bool KeyDown_Q = false;
    bool KeyDown_W = false;
    bool KeyDown_E = false;
    bool KeyDown_R = false;
    bool KeyDown_T = false;
    bool KeyDown_Y = false;
    bool KeyDown_U = false;
    bool KeyDown_I = false;
    bool KeyDown_O = false;
    bool KeyDown_P = false;


    bool KeyDown_A = false;
    bool KeyDown_S = false;
    bool KeyDown_D = false;
    bool KeyDown_F = false;
    bool KeyDown_G = false;
    bool KeyDown_H = false;
    bool KeyDown_J = false;
    bool KeyDown_K = false;
    bool KeyDown_L = false;


    bool KeyDown_Z = false;
    bool KeyDown_X = false;
    bool KeyDown_C = false;
    bool KeyDown_V = false;
    bool KeyDown_B = false;
    bool KeyDown_N = false;
    bool KeyDown_M = false;


    bool KeyDown_F1 = false;
    bool KeyDown_F2 = false;
    bool KeyDown_F3 = false;
    bool KeyDown_F4 = false;
    
    void Initialize();

  };

  using KeyboardPtr = std::unique_ptr<Keyboard>;
}