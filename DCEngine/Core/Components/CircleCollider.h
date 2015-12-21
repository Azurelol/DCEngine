/******************************************************************************/
/*!
@file   CircleCollider.h
@author Blaine Reiner@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The CircleCollider component gives a physical representation of a Circle
        in world space so objects can experience collisions with one 
        another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "Collider.h"

namespace DCEngine {
  namespace Components
  {
    class Transform;
    class CircleCollider : public Collider {
    public:

      float Radius = 5;
      Vec3 Offset = Vec3(0, 0, 0);
      Boolean Ghost = false;
      Boolean SendsEvents = true;
      Boolean IsDrawingCollider = false;

      float getRadius(void);
      Vec3 getOffset(void);
      bool getGhost(void);
      bool getSendsEvents(void);


      // draw collider is disabled because function only works for boxes right now
      //void DrawCollider();
      String getCollisionGroup() const;
      // These should be private!
      CircleCollider(Entity& owner) : Collider(owner, "CircleCollider") {}
      ~CircleCollider();
      void Initialize();

      void OnLogicUpdateEvent(Events::LogicUpdate* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);


    private:
      Transform* TransformComponent = NULL;
      CollisionGroupHandle CollisionGroup = String("Default");
    };
  }

}
