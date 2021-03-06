/******************************************************************************/
/*!
@file   HazardArea.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for HazardArea
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"
#include "../ReboundEvents.h"
#define HazardAreaTraceOn 0

namespace DCEngine {

  namespace Components
  {
    class Transform;
    class RigidBody;
    class Sprite;
    class HazardArea : public Component {

    public:

      Real Damage = 3.0f;
      Real DamageInterval = 1.0f;
      Real Timer = 0; //will always damage the player on first tick of contact
      bool TouchingPlayer = false;

      DCE_DEFINE_PROPERTY(Real, Damage);
      DCE_DEFINE_PROPERTY(Real, DamageInterval);

      HazardArea(Entity& owner) : Component(std::string("HazardArea"), owner) {}
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);
      void OnRespawnEvent(Events::RespawnEvent * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(HazardArea, Component);
#endif

    private:
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      GameObject* PlayerRef;

      void PrintTranslation();
      void PrintVelocity();
      std::string FootstepSound;
      Boolean FootstepSoundEnabled = false;

      SoundInstanceWeakPtr AcidHiss;

      void PlayerOutOfAcidNowPleaseStop();
    };
  }


}