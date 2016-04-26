/******************************************************************************/
/*!
@file   HazardArea.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Makes an area a hazard, which will damage the player on contact.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "HazardArea.h"
#include "../../CoreComponents.h"
#include "../ReboundEvents.h"

#define POSTEVENT(name) SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(name)

namespace DCEngine {
  namespace Components
  {
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(HazardArea, "HazardArea", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(HazardArea);
      DCE_BINDING_DEFINE_PROPERTY(HazardArea, Damage);
      DCE_BINDING_DEFINE_PROPERTY(HazardArea, DamageInterval);
    }
#endif

    void HazardArea::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(gameObj, Events::CollisionStarted, HazardArea::OnCollisionStartedEvent);
      Connect(gameObj, Events::CollisionEnded, HazardArea::OnCollisionEndedEvent);
      Connect(SpaceRef, Events::LogicUpdate, HazardArea::OnLogicUpdateEvent);
      Connect(SpaceRef, Events::RespawnEvent, HazardArea::OnRespawnEvent);
      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>(); // ew
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      PlayerRef = SpaceRef->FindObjectByName("Player");
      AcidHiss = nullptr;
    }

    void HazardArea::OnRespawnEvent(Events::RespawnEvent * event)
    {
      PlayerOutOfAcidNowPleaseStop();
    }
    
    void HazardArea::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<Components::PlayerController>())
      {
        TouchingPlayer = true;
        PlayerRef->getComponent<Components::RigidBody>()->setGravity(true);
        auto vel = PlayerRef->getComponent<Components::RigidBody>()->getVelocity();
        PlayerRef->getComponent<Components::RigidBody>()->setVelocity(Vec3(vel.x / 2, vel.y / 2, 0));
		    Timer = DamageInterval;
        AcidHiss = POSTEVENT("AcidHiss");
        if (HazardAreaTraceOn)
        {
          DCTrace << "HazardArea::OnCollisionStartedEvent \n";
        }
      }
    }

    void HazardArea::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
      if (event->OtherObject->getComponent<Components::PlayerController>())
      {
        PlayerOutOfAcidNowPleaseStop();
      }
    }

    void HazardArea::PlayerOutOfAcidNowPleaseStop()
    {
      TouchingPlayer = false;
      if (AcidHiss != nullptr)
      {
        AcidHiss->Stop();
      }
      PlayerRef->getComponent<Components::RigidBody>()->setGravity(1);
      Timer = 0;
      if (HazardAreaTraceOn)
      {
        DCTrace << "HazardArea::OnCollisionEndedEvent \n";
      }
    }

    void HazardArea::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      if (HazardAreaTraceOn)
      {
        DCTrace << "HazardArea::OnLogicUpdate - Timer = " << Timer << ", Dt = " << event->Dt << "\n";
      }
      if (TouchingPlayer)
      {
        Timer += event->Dt;
        if (Timer > DamageInterval)
        {
          DCTrace << "HazardArea::OnLogicUpdate - Timer Above " << DamageInterval << ", Timer =" << Timer << ", Dt = " << event->Dt << "\n";
          Timer = 0;
          PlayerRef->getComponent<Components::PlayerController>()->TakeDamage(static_cast<int>(Damage));
        }
      }
    }



    void HazardArea::PrintTranslation()
    {
      DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x << ", " << TransformRef->Translation.y << ", " << TransformRef->Translation.z << ")\n";
    }

    void HazardArea::PrintVelocity()
    {
      Vec3 vel = RigidBodyRef->getVelocity();
      //DCTrace << Owner()->Name() << "::RigidBody.Velocity(" << vel.x << ", " << vel.y<< ", " << vel.z << ")\n";
    }
  }

}