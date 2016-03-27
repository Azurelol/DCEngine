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
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"
#include "CollisionGroup.h"

namespace DCEngine {

  // Forward class declaration
  class GameObject;
  class CollisionTable;
  using CollisionTablePtr = std::shared_ptr<CollisionTable>;
  using CollisionTableHandle = std::string;

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
#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareBaseType(CollisionBlock, Zilch::TypeCopyMode::ValueType);
#endif
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
#if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareBaseType(CollisionFilter, Zilch::TypeCopyMode::ReferenceType);
#endif
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

    #if(DCE_USE_ZILCH_INTERNAL_BINDING) 
    ZilchDeclareDerivedType(CollisionTable, Resource);
    #endif

    CollisionTable(std::string name);
    CollisionTable(void);
    ~CollisionTable() {}
    static std::string Extension() { return ".CollisionTable"; }
    static std::string Default() { return "DefaultCollisionTable"; }

    bool AddGroup(CollisionGroup group);
    bool AddGroup(std::string group);
    std::vector<CollisionGroup> &GetGroups(void);
    std::vector<CollisionFilter> &GetPairs(void);
    void ScanForGroups(void);
    const CollisionFilter &GetFilter(std::string const &group1, std::string const &group2);
    bool SetResolve(std::string const &group1, std::string const &group2, CollisionFlag state);
    const CollisionFlag &GetResolve(std::string const &group1, std::string const &group2);
    bool SetStartBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    const CollisionBlock &GetStartBlock(std::string const &group1, std::string const &group2);
    bool SetEndBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    const CollisionBlock &GetEndBlock(std::string const &group1, std::string const &group2);
    bool SetPreSolveBlock(std::string const &group1, std::string const &group2, CollisionBlock state);
    const CollisionBlock &GetPreSolveBlock(std::string const &group1, std::string const &group2);

    CollisionTable &operator=(const CollisionTable & rhs);

    static CollisionTablePtr Find(std::string);


  private:

    std::vector<CollisionGroup> Groups;
    std::vector<CollisionFilter> Pairs;

  };



}