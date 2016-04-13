/******************************************************************************/
/*!
@file   Lancer.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   2/18/2016
@brief  An AI that charges its target
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Lancer.h"
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
    ZilchDefineType(Lancer, "Lancer", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Lancer);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, startingHealth);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, IsInvulnerable);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ShieldArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyShieldArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, HeadArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyHeadArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ShoulderArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyShoulderArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, BodyArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyBodyArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, SpearArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertySpearArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ChargeForce);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, ShieldReflectionForce);
      //DCE_BINDING_DEFINE_PROPERTY(Lancer, ShieldActivationSpeed);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationSpeedHead);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationDistanceHead);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationSpeedShoulder);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationDistanceShoulder);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationSpeedSpear);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, AnimationDistanceSpear);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, DamageTakenColor);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, DamageTakenColorFlashSpeed);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, KnockBackOnPlayerCollisionForce);
      DCE_BINDING_DEFINE_PROPERTY(Lancer, BallReflectForce);
    }

    // Dependancies
    DCE_COMPONENT_DEFINE_DEPENDENCIES(Lancer, "Transform", "RigidBody", "Sprite");
#endif

    Lancer::~Lancer()
    {
      delete stateMachine;
    }


    void Lancer::Initialize()
    {
      DCTrace << "Lancer Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Lancer::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Lancer::OnCollisionStartedEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();
      PhysicsSpaceRef = SpaceRef->getComponent<Components::PhysicsSpace>();
      GraphicsSpaceRef = SpaceRef->getComponent<Components::GraphicsSpace>();
      CollisionTableRef = Daisy->getSystem<Systems::Content>()->getCollisionTable(std::string(this->SpaceRef->getComponent<Components::PhysicsSpace>()->getCollisionTable()));
      CollisionTableRef->SetResolve("LancerShield", "Player", CollisionFlag::SkipDetecting);
      CollisionTableRef->SetResolve("LancerShield", "Ball", CollisionFlag::Resolve);
      CollisionTableRef->SetResolve("LancerShield", "Terrain", CollisionFlag::SkipDetecting);
      CollisionTableRef->SetResolve("LancerShield", "Enemy", CollisionFlag::SkipDetecting);

      stateMachine = new StateMachine<Lancer>(this);
      startingPosition = TransformRef->Translation;
      endPosition = startingPosition;

      stateMachine->SetGlobalState(Global::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);

      std::random_device rd;
      std::mt19937 generator(rd());
      std::uniform_real_distribution<float> distribution(0, 1);
      randomPhase = distribution(generator);


      SpriteRef->Visible = false;

      health = startingHealth;

      CreateSprites();

      Connect(shield, Events::CollisionStarted, Lancer::OnShieldCollisionStartedEvent);

      velocity = 0.0f;
      acceleration = 0.0f;
    }

    void Lancer::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();

      UpdateSprites(event->TimePassed);

      shield->getComponent<Transform>()->Translation = TransformRef->Translation;

      velocity += acceleration * event->Dt;
      TransformRef->Translation.x += velocity * event->Dt;
    }

    void Lancer::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        if (player->getComponent<PlayerController>()->Invincible)
        {
          stateMachine->ChangeState(Die::Instance());
        }
        else if (ModifyHealth(-1))
        {
          FlashColor(DamageTakenColor, DamageTakenColorFlashSpeed);
        }

        event->OtherObject->getComponent<RigidBody>()->ApplyForce(-event->Normal * BallReflectForce);
      }

      if (event->OtherObject->getComponent<PlayerController>() != NULL)
      {
        velocity = 0;
        RigidBodyRef->ApplyForce(Vec3(-KnockBackOnPlayerCollisionForce * acceleration,0,0));
      }
    }

    void Lancer::OnShieldCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->Name() == "Ball" &&
          event->OtherObject->getComponent<Components::BoxCollider>() &&
          event->OtherObject->getComponent<Components::BoxCollider>()->getGhost() != true)
      {
        Vec3 parentPosition = TransformRef->getTranslation();
        Vec3 otherPosition = event->OtherObject->getComponent<Components::Transform>()->getTranslation();

        //if (parentVelocity.x - otherVelocity.x > velocityDifferenceThreshold)
        //{
        //event->OtherObject->getComponent<Components::RigidBody>()->setVelocity(-otherVelocity);

        event->OtherObject->getComponent<Components::RigidBody>()->setVelocity(Vec3(0, 0, 0));
        event->OtherObject->getComponent<Components::RigidBody>()->ApplyForce(glm::normalize(otherPosition - parentPosition) * ShieldReflectionForce);
        //}
      }
    }

    bool Lancer::ModifyHealth(int amount)
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

    void Lancer::CreateSprites()
    {
      shield = SpaceRef->CreateObject(ShieldArchetype);
      head = SpaceRef->CreateObject(HeadArchetype);
      shoulder = SpaceRef->CreateObject(ShoulderArchetype);
      body = SpaceRef->CreateObject(BodyArchetype);
      spear = SpaceRef->CreateObject(SpearArchetype);
      sprites.push_back(shield);
      sprites.push_back(head);
      sprites.push_back(shoulder);
      sprites.push_back(body);
      sprites.push_back(spear);

      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        sprites.at(i)->AttachTo(gameObj);
        sprites.at(i)->getComponent<Transform>()->setLocalTranslation(Vec3(0, 0, 0));
      }

      head->getComponent<Transform>()->Translation.z = 0.01;
      shoulder->getComponent<Transform>()->Translation.z = 0.02;
      body->getComponent<Transform>()->Translation.z = 0;
      spear->getComponent<Transform>()->Translation.z = 0.03;
      shield->getComponent<Transform>()->Translation.z = 0.04;

    }

    void Lancer::UpdateSprites(float timePassed)
    {
      float y = sin((timePassed * AnimationSpeedHead) + randomPhase) * AnimationDistanceHead;
      head->getComponent<Transform>()->Translation.y += y;

      y = sin((timePassed * AnimationSpeedShoulder) + randomPhase) * AnimationDistanceShoulder;
      shoulder->getComponent<Transform>()->Translation.y += y;

      float x = sin((timePassed * AnimationSpeedSpear) + randomPhase) * AnimationDistanceSpear;
      spear->getComponent<Transform>()->Translation.x += x;
    }

    void Lancer::FlipSprites(bool flipX)
    {
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        sprites.at(i)->getComponent<Sprite>()->FlipX = flipX;
      }
    }

    void Lancer::FlashColor(Vec4 color, float duration)
    {
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        Vec4 oldColor = sprites.at(i)->getComponent<Sprite>()->Color;
        sprites.at(i)->getComponent<Sprite>()->Color = color;
        ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
        Actions::Property(seq, sprites.at(i)->getComponent<Sprite>()->Color, oldColor, duration, Ease::Linear);
      }
    }


#pragma region Global State
    void Lancer::Global::Enter(Lancer *owner) {}

    void Lancer::Global::Update(Lancer *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
      else
      {
        Ray leftRay;
        leftRay.Direction = Vec3(-1, 0, 0);
        leftRay.Origin = Vec3(owner->TransformRef->Translation.x, owner->TransformRef->Translation.y - (owner->TransformRef->Scale.y / 4), owner->TransformRef->Translation.z);
        CastFilter filter;
        filter.CollisionGroups.push_back(CollisionGroup::Find("Player"));
        filter.CollisionGroups.push_back(CollisionGroup::Find("Terrain"));
        filter.Include = true;
        CastResult castLeft = owner->PhysicsSpaceRef->CastRay(leftRay, filter);

        if (castLeft.ObjectHit == owner->player)
        {
          owner->stateMachine->ChangeState(ChargeLeft::Instance());
        }
       
        Ray rightRay;
        rightRay.Direction = Vec3(1, 0, 0);
        rightRay.Origin = Vec3(owner->TransformRef->Translation.x, owner->TransformRef->Translation.y - (owner->TransformRef->Scale.y / 4), owner->TransformRef->Translation.z);
        CastResult castRight = owner->PhysicsSpaceRef->CastRay(rightRay, filter);

        if (castRight.ObjectHit == owner->player)
        {
          owner->stateMachine->ChangeState(ChargeRight::Instance());
        }
      }

      //if (abs(owner->RigidBodyRef->getVelocity().x) > owner->ShieldActivationSpeed)
      //{
      //  //owner->shield->isActive = true;
      //  //owner->shield->SpriteRef->Visible = true;
      //}
      //else
      //{
      //  //owner->shield->isActive = false;
      //  //owner->shield->SpriteRef->Visible = false;
      //}
    }

    void Lancer::Global::Exit(Lancer *owner) {}

    Lancer::Global* Lancer::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Lancer::Idle::Enter(Lancer *owner)
    {
      //DCTrace << "Lancer Idle Enter\n";
      owner->acceleration = 0;
      owner->velocity = 0;
    }

    void Lancer::Idle::Update(Lancer *owner)
    {
      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);

      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Lancer::Idle::Exit(Lancer *owner)
    {

    }

    Lancer::Idle* Lancer::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region ChargeLeft State
    void Lancer::ChargeLeft::Enter(Lancer *owner)
    {
      //DCTrace << "Lancer ChargeLeft Enter\n";

      owner->FlipSprites(false);
    }

    void Lancer::ChargeLeft::Update(Lancer *owner)
    {
      owner->acceleration = -owner->ChargeForce;
      //owner->RigidBodyRef->setAcceleration(Vec3(-owner->ChargeForce, 0, 0));
      //owner->RigidBodyRef->ApplyForce(Vec3(-owner->ChargeForce, 0, 0));
    }

    void Lancer::ChargeLeft::Exit(Lancer *owner)
    {

    }

    Lancer::ChargeLeft* Lancer::ChargeLeft::Instance()
    {
      static ChargeLeft instance;
      return &instance;
    }
#pragma endregion ChargeLeft State

#pragma region ChargeRight State
    void Lancer::ChargeRight::Enter(Lancer *owner)
    {
      DCTrace << "Lancer ChargeRight Enter\n";

      owner->FlipSprites(true);
    }

    void Lancer::ChargeRight::Update(Lancer *owner)
    {
      owner->acceleration = owner->ChargeForce;
      //owner->RigidBodyRef->setAcceleration(Vec3(owner->ChargeForce, 0, 0));
      //owner->RigidBodyRef->ApplyForce(Vec3(owner->ChargeForce, 0, 0));
    }

    void Lancer::ChargeRight::Exit(Lancer *owner)
    {

    }

    Lancer::ChargeRight* Lancer::ChargeRight::Instance()
    {
      static ChargeRight instance;
      return &instance;
    }
#pragma endregion ChargeRight State

#pragma region Die State
    void Lancer::Die::Enter(Lancer *owner)
    {
      //DCTrace << "Lancer Die Enter\n";

      owner->acceleration = 0;
      owner->velocity = 0;
      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Lancer::Die::Update(Lancer *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Lancer::Die::Exit(Lancer *owner)
    {
      // Destroy Lancer
      owner->gameObj->Destroy();
    }

    Lancer::Die* Lancer::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}