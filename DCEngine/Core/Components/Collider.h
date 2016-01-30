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

namespace DCEngine {
  namespace Components {

    class Transform;
    class Physics;
    class Collider : public Component {
    public:
      friend class Physics;

#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(Collider, Component);
#endif

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
      Collider(Entity& owner, std::string colliderClass);
      ~Collider();
      virtual void Initialize();
      //Vec3 getColliderScale();

			std::vector<Collider*>* bucket;

    private:

      // Transform* TransformComponent;
      //
      // // The group is a tag used for filter out collisions. The rules
      // // for filtering are on the CollisionFilter that is on the running space.
      CollisionGroupHandle CollisionGroup = String("Default");

			//for quad trees
			

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
