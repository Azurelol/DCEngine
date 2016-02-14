/*****************************************************************************/
/*!
@file   HealthController.cpp
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   2/6/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "HealthController.h"
#include "../../CoreComponents.h"
#include "../ReboundEvents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(HealthController, "HealthController", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(HealthController);
      DCE_BINDING_DEFINE_PROPERTY(HealthController, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(HealthController, startingHealth);
    }
#endif


    void HealthController::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());

      health = startingHealth;
    }

    /**************************************************************************/
    /*!
    @brief Changes the health value by amount. Negative amount will decrease 
           health, positive amount will increase health. Will keep health 
           between maxHealth and 0.
    @param The amount to change health by
    @return Whether the health was actually changed
    */
    /**************************************************************************/
    bool HealthController::ModifyHealth(int amount)
    {
      int oldHealth = health;

      if (!IsInvulnerable)
      {
        health += amount;

        if (health > maxHealth)
          health = maxHealth;
        if (health < 0)
          health = 0;
      }

      if (health == 0)
      {
        Events::DeathEvent *e;
        Owner()->Dispatch<Events::DeathEvent>(e);
      }

      if (oldHealth == health)
        return false;
      else
        return true;
    }

    int HealthController::GetHealth() const
    {
      return health;
    }
  }
}