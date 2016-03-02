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
#pragma once
#include "../ReboundComponent.h"
#include "../../../../Core/Systems/StateMachine/StateMachine.h"
#include "../../ReboundEvents.h"

namespace DCEngine {
  namespace Components {
    class Transform;
    class RigidBody;
    class Sprite;
    class HealthController;
    class PhysicsSpace;
    class GraphicsSpace;
    class Lancer : public Component
    {

    public:
      GameObject* gameObj;
      Transform* TransformRef;
      RigidBody* RigidBodyRef;
      Sprite* SpriteRef;
      HealthController* HealthRef;
      PhysicsSpace* PhysicsSpaceRef;
      CollisionTablePtr CollisionTableRef;
      GraphicsSpace* GraphicsSpaceRef;
      String PlayerName = "Player";
      float IdleRange;        // Past this range, the grunt will be idle, within the range, it will patrol
      float ChargeForce;



      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Properties
      DCE_DEFINE_PROPERTY(String, PlayerName);
      DCE_DEFINE_PROPERTY(float, IdleRange);
      DCE_DEFINE_PROPERTY(float, ChargeForce);

      // Methods
      Lancer(Entity& owner) : Component(std::string("Lancer"), owner) {}
      ~Lancer();
      void Initialize();
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);
      void OnDeathEvent(Events::DeathEvent * event);

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Lancer, Component);
#endif

    private:
      StateMachine<Lancer> *stateMachine;
      GameObject *player;
      Vec3 startingPosition;
      Vec3 endPosition;
      float jumpTimer = 0;
      float dt;

      void Jump(int direction, float period, float strengthX, float strengthY);

      class Global : public IState<Lancer>
      {
      public:
        static Global* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);

      private:
        Global() {};
      };

      class Idle : public IState<Lancer>
      {
      public:
        static Idle* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        Idle() {};
      };

      class ChargeLeft : public IState<Lancer>
      {
      public:
        static ChargeLeft* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        ChargeLeft() {};
      };

      class ChargeRight : public IState<Lancer>
      {
      public:
        static ChargeRight* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        ChargeRight() {};
      };

      class Die : public IState<Lancer>
      {
      public:
        static Die* Instance();
        void Enter(Lancer *owner);
        void Update(Lancer *owner);
        void Exit(Lancer *owner);
      private:
        Die() {};
      };
    };
  }

}