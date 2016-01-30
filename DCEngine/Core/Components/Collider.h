/******************************************************************************/
/*!
@file   Collider.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Resources/PhysicsMaterial.h"

namespace DCEngine  {

  namespace Systems {
    class Editor;
  }

  namespace Components {

    class Transform;
    class Physics;

    class Collider : public Component {
    public:
      friend class Physics;

      static unsigned CollidersCreated;
      static unsigned CollidersDestroyed;
      static unsigned CollidersActive;

      /* Variables */
      //Vec3 Size = Vec3(1, 1, 1);
      //Vec3 Offset = Vec3(0, 0, 0);
      //Boolean Ghost = false;
      //Boolean SendsEvents = true;
      //Boolean IsDrawingCollider = false;


      /* Properties */
      DCE_DEFINE_PROPERTY(String, CollisionGroup);

      // @todo For now it's a string, it will be later a tagged property so there's a dropdown menu in the editor
      //String getCollisionGroup() const;
      //void setCollisionGroup(String);

      // These should be private!
      ZilchDeclareDerivedType(Collider, Component);
      Collider(Entity& owner, std::string colliderClass);
      ~Collider();
      virtual void Initialize();


    private:

      // Transform* TransformComponent;
      //
      // // The group is a tag used for filter out collisions. The rules
      // // for filtering are on the CollisionFilter that is on the running space.
      CollisionGroupHandle CollisionGroup = String("Default");



      /* Events */
      //void OnLogicUpdateEvent(Events::LogicUpdate* event);
      //void OnCollisionStartedEvent(Events::CollisionStarted* event);
      //void OnCollisionEndedEvent(Events::CollisionEnded* event);
      ///* Methods */
      //void DrawCollider();

      /* Variables */


    };

  }
}
