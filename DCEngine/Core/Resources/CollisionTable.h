/*****************************************************************************/
/*!
@file   CollisionTable.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu, blaine.reiner\@digipen.edu
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
#include "CollisionGroup.h"

namespace DCEngine {

  // Forward class declaration
  class GameObject;

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
    bool SendEventsToA = true;
    //!< Whether or not to send events to the object of TypeB in a collision
    //   of TypeA and TypeB
    bool SendEventsToB = true;
    //!< Wheter or not to send events to the space in a collision of TypeA and
    //   and TypeB.
    bool SendEventsToSpace = true;
    //!< What event name to send out. If left empty the event name will be
    //   'Events.GroupCollision(Started/Ended/Persisted)'

  };

  struct CollisionFilter {
    // the pair of groups
    std::pair<std::string, std::string> Pairing;
    // Controls the resolution of collision
    CollisionFlag CollisionFlag = CollisionFlag::Resolve;
    // Controls what events to send when a collision starts between TypeA and TypeB
    CollisionBlock CollisionStartBlock;
    // Controls what events to send when a collision ends between TypeA and TypeB
    CollisionBlock CollisionEndBlock;
    // Sent before collision is resolved between TypeA and TypeB
    CollisionBlock PreSolveBlock;
  };

  class CollisionTable : public Resource {

  public:
    CollisionTable(std::string name);
    ~CollisionTable() {}

    bool AddGroup(CollisionGroup group);
    bool AddGroup(std::string group);
    std::vector<CollisionGroup> const &GetGroups(void) const;
    std::vector<CollisionFilter> const &GetPairs(void) const;
    CollisionFilter &GetFilter(std::string const &group1, std::string const &group2);
    bool SetResolve(std::string const &group1, std::string const &group2, CollisionFlag state);
    CollisionFlag &GetResolve(std::string const &group1, std::string const &group2);
    bool SetStartBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    CollisionBlock &GetStartBlock(std::string const &group1, std::string const &group2);
    bool SetEndBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    CollisionBlock &GetEndBlock(std::string const &group1, std::string const &group2);
    bool SetPreSolveBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    CollisionBlock &GetPreSolveBlock(std::string const &group1, std::string const &group2);

  private:

    std::vector<CollisionGroup> Groups;

    std::vector<CollisionFilter> Pairs;

  };

  using CollisionTableHandle = std::string;

}