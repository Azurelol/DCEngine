/*****************************************************************************/
/*!
@file   CollisionGroup.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/19/2015
@brief  Collision Tables are used to specify what kinds of objects can/can’t 
        collide with each other. Tables can also be used to send specific events
        to one of the objects when a collision happens between a pair. Collision 
        groups are used as a tag to identify an object in the table.
@note   Reference and inspiration:
        http://zero.digipen.edu/ZeroManual/Physics/Collision/CollisionTables.html?highlight=collision%20table
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {

  // Forward class declaration
  class GameObject;

  class CollisionTable : public Resource {

    enum class CollisionFlag {
      // Don't detect collision between this pair 
      // (no collision events will be set.) Used as an optimization flag
      SkipDetecting, 
      // Don't resolve collision (physics) but still send events
      SkipResolution,
      // Resolve collisions as normal and send events
      Resolve,
    };

    struct CollisionBlock {
      //!< Whether or not to send events to the object of TypeA in a collision 
      //   of TypeA and TypeB
      bool SendEventsToA;
      //!< Whether or not to send events to the object of TypeB in a collision 
      //   of TypeA and TypeB
      bool SendEventsToB;
      //!< Wheter or not to send events to the space in a collision of TypeA and
      //   and TypeB.
      bool SendEventsToSpace;
      //!< What event name to send out. If left empty the event name will be
      //   'Events.GroupCollision(Started/Ended/Persisted)'
                      
    };

    struct CollisionFilter {
      //std::pair<GameObject*, GameObject*>;
      CollisionFlag CollisionFlag;
      // Controls what events to send when a collision starts between TypeA and TypeB
      CollisionBlock CollisionStartBlock; 
      // Controls what events to send when a collision ends between TypeA and TypeB
      CollisionBlock CollisionEndBlock;
      // Sent before collision is resolved between TypeA and TypeB
      CollisionBlock PreSolveBlock;
    };

  public:
    CollisionTable(std::string name) : Resource(name) {}
    ~CollisionTable() {}

  private:

  };

  using CollisionTableHandle = std::string;

}