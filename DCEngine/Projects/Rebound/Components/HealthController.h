/*****************************************************************************/
/*!
@file   HealthController.h
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   2/6/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
  namespace Components {
    class HealthController : public Component {

    public:
      bool IsInvulnerable = false;

      // Properties
      DCE_DEFINE_PROPERTY(int, maxHealth);
      DCE_DEFINE_PROPERTY(int, startingHealth);
      DCE_DEFINE_PROPERTY(bool, IsInvulnerable);

      // Methods
      HealthController(Entity& owner) : Component(std::string("HealthController"), owner) {}
      void Initialize();
      bool ModifyHealth(int amount); //Changes the health value by amount. Negative amount will decrease health, positive amount will increase health.Will keep health between maxHealth and 0.
      int GetHealth() const; 

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(HealthController, Component);
#endif

    private:
      // Member variables
      int maxHealth;
      int startingHealth;
      int health;
    };
  }

}