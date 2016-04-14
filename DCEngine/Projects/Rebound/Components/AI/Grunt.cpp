/******************************************************************************/
/*!
@file   Grunt.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  An AI that patrols on a set route until the player enter's it's area.
        Then it will attack until the player leaves.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Grunt.h"
#include "../../../CoreComponents.h"

#define POSTEVENT(name) SpaceRef->getComponent<Components::SoundSpace>()->PlayCue(name)

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Grunt, "Grunt", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Grunt);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PlayerName);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, startingHealth);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, maxHealth);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsInvulnerable);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, HeadArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyHeadArchetype, attributeArchetype);
      //DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, BodyArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyBodyArchetype, attributeArchetype);
     // DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(Archetype);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, SawArchetype);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertySawArchetype, attributeArchetype);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IdleRange);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PatrolDistance);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsPatrolRight);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, IsAggressive);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, JumpPeriod);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthX);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpStrengthY);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackJumpPeriod);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AnimationSpeedHead);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AnimationDistanceHead);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AnimationSpeedSaw);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AnimationDistanceSaw);
      //DCE_BINDING_DEFINE_PROPERTY(Grunt, IdleColor);
      //DCE_BINDING_DEFINE_PROPERTY(Grunt, PatrolColor);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, AttackColor);
      //DCE_BINDING_DEFINE_PROPERTY(Grunt, IsDebugColorActive);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, DamageTakenColor);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, DamageTakenColorFlashSpeed);
      DCE_BINDING_DEFINE_PROPERTY(Grunt, BallReflectForce);
    }

    // Dependancies
    DCE_COMPONENT_DEFINE_DEPENDENCIES(Grunt, "Transform", "RigidBody", "Sprite");
#endif

    Grunt::~Grunt()
    {
      delete stateMachine;
    }


    void Grunt::Initialize()
    {
      DCTrace << "Grunt Initialize\n";
      gameObj = dynamic_cast<GameObject*>(Owner());
      Connect(SpaceRef, Events::LogicUpdate, Grunt::OnLogicUpdateEvent);
      Connect(gameObj, Events::CollisionStarted, Grunt::OnCollisionStartedEvent);

      TransformRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Transform>();
      RigidBodyRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::RigidBody>();
      SpriteRef = dynamic_cast<GameObject*>(Owner())->getComponent<Components::Sprite>();

      stateMachine = new StateMachine<Grunt>(this);
      startingPosition = TransformRef->Translation;
      endPosition = startingPosition;

      health = startingHealth;

      defaultColor = SpriteRef->Color;
 
      std::random_device rd;
      std::mt19937 generator(rd());
      std::uniform_real_distribution<float> distribution(0, 1);
      float rand = distribution(generator);

      if (IsPatrolRight)
      {
        endPosition.x = startingPosition.x + PatrolDistance;
        ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
        Actions::Delay(seq, rand);
        Actions::Call(seq, &Grunt::ChangeStateRight, this);
      }
      else
      {
        endPosition.x = startingPosition.x - PatrolDistance;
        ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
        Actions::Delay(seq, rand);
        Actions::Call(seq, &Grunt::ChangeStateLeft, this);
      }


      stateMachine->SetGlobalState(Global::Instance());

      player = SpaceRef->FindObjectByName(PlayerName);

      if(SpriteRef)
        SpriteRef->Visible = false;
      
      randomPhase = distribution(generator);
      CreateSprites();

	    particle = SpaceRef->CreateObject("ScrapperParticle");
      particle->AttachTo(gameObj);
	    if (particle)
	    {
		    particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation + ParticleOffset + Vec3(RigidBodyRef->getVelocity().x / 60, 0 ,0));
	    }
    }

    void Grunt::ChangeStateRight()
    {
      stateMachine->ChangeState(PatrolRight::Instance());
    }

    void Grunt::ChangeStateLeft()
    {
      stateMachine->ChangeState(PatrolLeft::Instance());
    }

    void Grunt::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
      dt = event->Dt;
      UpdateSprites(event->TimePassed);
	    particle->getComponent<Components::Transform>()->setTranslation(TransformRef->Translation + ParticleOffset + Vec3(RigidBodyRef->getVelocity().x / 60, 0, 0));
    }

    void Grunt::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      if (event->OtherObject->getComponent<BallController>() != NULL)
      {
        if (player->getComponent<PlayerController>()->Invincible)
        {
          POSTEVENT("ScrapperDeath");
          stateMachine->ChangeState(Die::Instance());
        }
        else if (ModifyHealth(-1))
        {
          FlashColor(DamageTakenColor, DamageTakenColorFlashSpeed);
        }

        event->OtherObject->getComponent<RigidBody>()->ApplyForce(-event->Normal * BallReflectForce);
      }
      else if (event->OtherObject->getComponent<PlayerController>() != NULL)
      {
        POSTEVENT("ScrapperAttack");
      }
    }

    bool Grunt::ModifyHealth(int amount)
    {
      int oldHealth = health;

      if (!IsInvulnerable)
      {
        if (isDamageable)
        {
          health += amount;

          if (health > maxHealth)
            health = maxHealth;
          if (health < 0)
            health = 0;

          isDamageable = false;
          ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
          Actions::Property(seq, isDamageable, true, DamageTakenColorFlashSpeed + 0.3f, Ease::Linear);
        }
      }

      if (health == 0)
      {
        POSTEVENT("ScrapperDeath");
        stateMachine->ChangeState(Die::Instance());
      }
      else
      {
        POSTEVENT("ScrapperTakeDamage");
      }

      if (oldHealth == health)
        return false;
      else
        return true;
    }

    void Grunt::CreateSprites()
    {
      head = SpaceRef->CreateObject(HeadArchetype);
      body = SpaceRef->CreateObject(BodyArchetype);
      saw = SpaceRef->CreateObject(SawArchetype);
      sprites.push_back(head);
      sprites.push_back(body);
      sprites.push_back(saw);
    
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        sprites.at(i)->AttachTo(gameObj);
        sprites.at(i)->getComponent<Transform>()->SetLocalTranslation(Vec3(0, 0, 0));
      }

      head->getComponent<Transform>()->Translation.z = 0.01;
      body->getComponent<Transform>()->Translation.z = 0;
      saw->getComponent<Transform>()->Translation.z = 0.02;
    }

    void Grunt::UpdateSprites(float timePassed)
    {

      float y = sin((timePassed * AnimationSpeedHead) + randomPhase) * AnimationDistanceHead;
      //DCTrace << y << " Time Passed: " << timePassed << "\n";

      head->getComponent<Transform>()->Translation.y += y;
      
      float x = sin((timePassed * AnimationSpeedSaw) + randomPhase) * AnimationDistanceSaw;
      saw->getComponent<Transform>()->Translation.x += x;
    }

    void Grunt::FlipSprites(bool flipX)
    {
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        sprites.at(i)->getComponent<Sprite>()->FlipX = flipX;
      }
    }

    void Grunt::ChangeSpritesColor(Vec4 newColor)
    {
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        sprites.at(i)->getComponent<Sprite>()->Color = newColor;
      }
    }

    void Grunt::FlashColor(Vec4 color, float duration)
    {
      for (unsigned i = 0; i < sprites.size(); ++i)
      {
        Vec4 oldColor = sprites.at(i)->getComponent<Sprite>()->Color;
        sprites.at(i)->getComponent<Sprite>()->Color = color;
        ActionSetPtr seq = Actions::Sequence(Owner()->Actions);
        Actions::Property(seq, sprites.at(i)->getComponent<Sprite>()->Color, oldColor, duration, Ease::Linear);
      }
    }

    // Direction should be 1 (right) or -1 (left). 
    void Grunt::Jump(int direction, float period, float strengthX, float strengthY)
    {
      if (jumpTimer > period)
      {
        RigidBodyRef->setVelocity(RigidBodyRef->getVelocity() + Vec3(strengthX *  direction, strengthY, 0));
        jumpTimer = 0;

        particle->getComponent<ParticleEmitter>()->Size = 0.8f;
      }

      jumpTimer += dt;

      if(RigidBodyRef->getVelocity().y < 0)
        particle->getComponent<ParticleEmitter>()->Size = 0.3f;

    }

#pragma region Global State
    void Grunt::Global::Enter(Grunt *owner){}

    void Grunt::Global::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if ((distanceFromPlayer > owner->IdleRange) && !owner->stateMachine->isInState(Idle::Instance()))
        owner->stateMachine->ChangeState(Idle::Instance());
    }

    void Grunt::Global::Exit(Grunt *owner){}

    Grunt::Global* Grunt::Global::Instance()
    {
      static Global instance;
      return &instance;
    }
#pragma endregion Global State

#pragma region Idle State
    void Grunt::Idle::Enter(Grunt *owner)
    {
      //if(owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->IdleColor;

      DCTrace << "Grunt Idle Enter\n";
    }

    void Grunt::Idle::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 playerPosition = owner->player->getComponent<Components::Transform>()->Translation;
      Vec3 ownerPosition = owner->TransformRef->Translation;
      float distanceFromPlayer = glm::distance(playerPosition, ownerPosition);
      
      if (distanceFromPlayer < owner->IdleRange)
        owner->stateMachine->RevertToPreviousState();
    }

    void Grunt::Idle::Exit(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->defaultColor;
    }

    Grunt::Idle* Grunt::Idle::Instance()
    {
      static Idle instance;
      return &instance;
    }
#pragma endregion Idle State

#pragma region Right State
    void Grunt::PatrolRight::Enter(Grunt *owner)
    {
      owner->jumpTimer = 0;

      //if (owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->PatrolColor;

      owner->FlipSprites(true);
      DCTrace << "Grunt PatrolRight Enter\n";
    }

    void Grunt::PatrolRight::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);
      
      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if(ownerPosition.x > owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolLeft::Instance());
      }        
    }

    void Grunt::PatrolRight::Exit(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->defaultColor;

      //owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolRight* Grunt::PatrolRight::Instance()
    {
      static PatrolRight instance;
      return &instance;
    }
#pragma endregion Right State

#pragma region Left State
    void Grunt::PatrolLeft::Enter(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->PatrolColor;

      owner->FlipSprites(false);
      DCTrace << "Grunt PatrolLeft Enter\n";
    }

    void Grunt::PatrolLeft::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      owner->Jump(-1, owner->JumpPeriod, owner->JumpStrengthX, owner->JumpStrengthY);

      if (owner->IsPatrolRight)
      {
        if (playerPosition.x > owner->startingPosition.x && playerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->startingPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
      else
      {
        if (playerPosition.x < owner->startingPosition.x && playerPosition.x > owner->endPosition.x)
          owner->stateMachine->ChangeState(Attack::Instance());
        else if (ownerPosition.x < owner->endPosition.x)
          owner->stateMachine->ChangeState(PatrolRight::Instance());
      }
    }

    void Grunt::PatrolLeft::Exit(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
        //owner->SpriteRef->Color = owner->defaultColor;

      //owner->RigidBodyRef->setVelocity(Vec3());
    }

    Grunt::PatrolLeft* Grunt::PatrolLeft::Instance()
    {
      static PatrolLeft instance;
      return &instance;
    }
#pragma endregion Left State

#pragma region Attack State
    void Grunt::Attack::Enter(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
      owner->ChangeSpritesColor(owner->AttackColor);

      //DCTrace << "Grunt Attack Enter\n";
    }

    void Grunt::Attack::Update(Grunt *owner)
    {
      // If there is no player, do nothing
      if (!owner->player)
        return;

      Vec3 ownerPosition = owner->TransformRef->Translation;
      Vec3 playerPosition = owner->player->getComponent<Transform>()->Translation;
      Vec3 direction = playerPosition - ownerPosition;
      if (direction.x < 0)
      {
        owner->FlipSprites(false);
        owner->Jump(-1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);
      }
      else
      {
        owner->FlipSprites(true);
        owner->Jump(1, owner->AttackJumpPeriod, owner->AttackJumpStrengthX, owner->AttackJumpStrengthY);
      }

      if (!owner->IsAggressive)
      {
        if (owner->IsPatrolRight)
        {
          if (playerPosition.x < owner->startingPosition.x || playerPosition.x > owner->endPosition.x)
            owner->stateMachine->RevertToPreviousState();
        }
        else
        {
          if (playerPosition.x > owner->startingPosition.x || playerPosition.x < owner->endPosition.x)
            owner->stateMachine->RevertToPreviousState();
        }
      }
    }

    void Grunt::Attack::Exit(Grunt *owner)
    {
      //if (owner->IsDebugColorActive)
      owner->ChangeSpritesColor(owner->defaultColor);
    }

    Grunt::Attack* Grunt::Attack::Instance()
    {
      static Attack instance;
      return &instance;
    }
#pragma endregion Attack State

#pragma region Die State
    void Grunt::Die::Enter(Grunt *owner)
    {
      //DCTrace << "Grunt Die Enter\n";

      owner->RigidBodyRef->setVelocity(Vec3(0, 0, 0));

      // Death sound?
    }

    void Grunt::Die::Update(Grunt *owner)
    {
      // Timer for death animation?

      Exit(owner);
    }

    void Grunt::Die::Exit(Grunt *owner)
    {
    //create death particle
    auto particle = owner->SpaceRef->CreateObject("EnemyExplosionParticle");
    if (particle)
    {
      particle->getComponent<Components::Transform>()->setTranslation(owner->TransformRef->Translation); 
    }
      // Destroy grunt

      owner->gameObj->Destroy();
    }

    Grunt::Die* Grunt::Die::Instance()
    {
      static Die instance;
      return &instance;
    }
#pragma endregion Die State

  }
}