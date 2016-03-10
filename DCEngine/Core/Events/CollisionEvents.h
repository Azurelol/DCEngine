/******************************************************************************/
/*!
\file   CollisionEvents.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/27/2015
\brief  Collision events sent by the Physics system on its resolution phase.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "EventReference.h"

namespace DCEngine {

  /* Forward declarations */
  class GameObject;

  namespace Events
  {
    class CollisionStarted : public Event {
    public:
      ZilchDeclareDerivedType(CollisionStarted, Event);
      CollisionStarted() : Event("CollisionStarted") {}
      GameObject* Object; //!< The object this event was sent to.
      GameObject* OtherObject; //!< The other object in the collision.
      Boolean IsGhost;
      // ContactPoint Data:
      // Number of contact points
      // A range of contact points
      // The first contact point

    };

    class CollisionEnded : public Event {
    public:
      ZilchDeclareDerivedType(CollisionEnded, Event);
      CollisionEnded() : Event("CollisionEnded") {}
      GameObject* Object; //!< The object this event was sent to.
      GameObject* OtherObject; //!< The other object in the collision.
      Boolean IsGhost;
    };

    class CollisionPersisted : public Event {
    public:
      ZilchDeclareDerivedType(CollisionPersisted, Event);
      CollisionPersisted() : Event("CollisionPersisted") {}
      GameObject* Object; //!< The object this event was sent to.
      GameObject* OtherObject; //!< The other object in the collision.
      Boolean IsGhost;
    };
  }



}