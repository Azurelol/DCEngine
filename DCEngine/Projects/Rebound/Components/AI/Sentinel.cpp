/******************************************************************************/
/*!
@file   Sentinel.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   2/18/2016
@brief  An AI with a shield that blocks head-on attacks
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Sentinel.h"
#include "../../../CoreComponents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Sentinel, "Sentinel", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Sentinel);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, startingHealth);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, IsInvulnerable);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, MoveSpeed);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyShieldArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldRadius);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldBashDistance);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldBashOutTime);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldBashInTime);
      DCE_BINDING_DEFINE_PROPERTY(Sentinel, ShieldBashCooldown);
    }

    DCE_COMPONENT_DEFINE_DEPENDENCIES(Sentinel, "Transform", "RigidBody", "Sprite");
#endif

    Sentinel::~Sentinel()
    {
      delete stateMachine;
    }


    void Sentinel::Initialize()
    {
      DCTrace << "Sentinel Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Sentinel::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Sentinel::OnCollisionStartedEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
      PhysicsSpaceRef = SpaceRef->getComponent<Components::PhysicsSpace>();

      stateMachine = new StateMachine<Sentinel>(this);

      stateMachine->SetGlobalState(Global::Instance());
      stateMachine->SetCurrentState(Idle::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);

      
      shieldLocalTranslation = Vec3(2, 0, 0);
      isBashing = false;
      canBash = true;
      CreateShield();
    }

    void Sentinel::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
      dt = event->Dt;

      if(!isBashing)
        UpdateShield();

      shield->getComponent<Transform>()->SetLocalTranslation(shieldLocalTranslation);
    }

    void Sentinel::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        ModifyHealth(-1);
      }
    }

    bool Sentinel::ModifyHealth(int amount)
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
        stateMachine->ChangeState(Die::Instance());
      }

      if (oldHealth == health)
        return false;
      else
        return true;
    }

    void Sentinel::CreateShield()
    {
      shield = SpaceRef->CreateObject(ShieldArchetype);
      shield->AttachTo(gameObj);
      shield->getComponent<Transform>()->SetLocalTranslation(shieldLocalTranslation);
      shield->getComponent<Orientation>()->WorldForward = Vec3(1, 0, 0);
      shield->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);
      CollisionTablePtr CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      CollisionTableRef->SetResolve("Enemy", "SentinelShield", CollisionFlag::SkipDetecting);
    }

    void Sentinel::UpdateShield()
    {
      //DCTrace << "Sentinel Shield Mass: " << shield->getComponent<BoxCollider>()->GetMass();

      Vec3 playerPosition = player->getComponent<Transform>()->Translation;
      Vec3 thisPosition = TransformRef->Translation;


      shield->getComponent<Orientation>()->LookAtPoint(playerPosition);

      Vec3 playerDirection = glm::normalize(playerPosition - thisPosition);
      

      shieldLocalTranslation = playerDirection * ShieldRadius;
      
      shield->getComponent<Transform>()->SetLocalTranslation(shieldLocalTranslation);
    }

    void Sentinel::ShieldBash()
    {
      if (!canBash)
        return;

      Vec3 playerPosition = player->getComponent<Transform>()->Translation;
      Vec3 thisPosition = TransformRef->Translation;

      Vec3 playerDirection = glm::normalize(playerPosition - thisPosition);
      
      ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
      Actions::Property(seq, isBashing, true, 0.0f, Ease::Linear);
      Actions::Property(seq, canBash, false, 0.0f, Ease::Linear);
      Actions::Property(seq, shieldLocalTranslation, (ShieldRadius + ShieldBashDistance) * playerDirection, ShieldBashOutTime, Ease::QuadOut);
      Actions::Property(seq, shieldLocalTranslation, ShieldRadius * playerDirection, ShieldBashInTime, Ease::Linear);
      Actions::Property(seq, isBashing, false, 0.0f, Ease::Linear);
      Actions::Property(seq, canBash, true, ShieldBashCooldown, Ease::Linear);
    }


#pragma region Global State
    void Sentinel::Global::Enter(Sentinel *owner) {}

    void Sentinel::Global::Update(Sentinel *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if ((distanceFromPlayer < (owner->ShieldBashDistance + owner->ShieldRadius)))
      {
        Ray ray;
        ray.Direction = Vec3(-1, 0, 0);
        ray.Origin = owner->TransformRef->Translation;
        CastFilter filter;
        filter.CollisionGroups.push_back(CollisionGroup::Find("Player"));
        filter.CollisionGroups.push_back(CollisionGroup::Find("Terrain"));
        filter.Include = true;
        CastResult castLeft = owner->PhysicsSpaceRef->CastRay(ray, filter);

        if(castLeft.ObjectHit == owner->player)
          owner->ShieldBash();

        ray.Direction = Vec3(1, 0, 0);
        CastResult castRight = owner->PhysicsSpaceRef->CastRay(ray, filter);

        if (castRight.ObjectHit == owner->player)
          owner->ShieldBash();
      }

      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());

    }

    void Sentinel::Global::Exit(Sentinel *owner) {}

    Sentinel::Global* Sentinel::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Sentinel::Idle::Enter(Sentinel *owner)
    {
      //DCTrace << "Sentinel Idle Enter\n";
    }

    void Sentinel::Idle::Update(Sentinel *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->ChangeState(Attack::Instance());
    }

    void Sentinel::Idle::Exit(Sentinel *owner)
    {

    }

    Sentinel::Idle* Sentinel::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region Attack State
    void Sentinel::Attack::Enter(Sentinel *owner)
    {

    }

    void Sentinel::Attack::Update(Sentinel *owner)
    {
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      Vec3 direction = playerPosition - ownerPosition;

      Ray ray;
      ray.Direction = glm::normalize(Vec3(direction.x, 0, 0));
      ray.Origin = owner->TransformRef->Translation;
      CastFilter filter;
      filter.CollisionGroups.push_back(CollisionGroup::Find("Terrain"));
      filter.Include = true;
      CastResult cast = owner->PhysicsSpaceRef->CastRay(ray, filter);

      if (cast.Distance > owner->ShieldRadius + owner->ShieldBashDistance + owner->player->getComponent<BoxCollider>()->getColliderScale().x)
      {
        if (direction.x < 0)
        {
          owner->RigidBodyRef->setVelocity(Vec3(-owner->MoveSpeed, owner->RigidBodyRef->getVelocity().y, 0));
          owner->SpriteRef->FlipX = false;
          owner->shield->getComponent<Sprite>()->FlipX = false;
        }
        else
        {
          owner->RigidBodyRef->setVelocity(Vec3(owner->MoveSpeed, owner->RigidBodyRef->getVelocity().y, 0));
          owner->SpriteRef->FlipX = true;
          owner->shield->getComponent<Sprite>()->FlipX = true;
        }
      }

      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      if (distanceFromPlayer > owner->IdleRange)
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Sentinel::Attack::Exit(Sentinel *owner)
    {
      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));
    }

    Sentinel::Attack* Sentinel::Attack::Instance()
    {
      static Attack instance;
      return &instance;
    }

#pragma endregion Attack State

#pragma region Die State
    void Sentinel::Die::Enter(Sentinel *owner)
    {
      //DCTrace << "Sentinel Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Sentinel::Die::Update(Sentinel *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Sentinel::Die::Exit(Sentinel *owner)
    {
      // Destroy sentinel
      owner->gameObj->Destroy();
    }

    Sentinel::Die* Sentinel::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}