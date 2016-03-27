/*****************************************************************************/
/*!
@file   CollisionTable.cpp
@author Blaine Reiner
@par    email: blaine.reiner\@digipen.edu
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
#include "CollisionTable.h"

// Engine
#include "../../Core/Engine/Engine.h"

namespace DCEngine
{

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/


#if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(CollisionBlock, "CollisionBlock", DCEngineCore, builder, type) {
    ZilchBindField(builder, type, &CollisionBlock::SendEventsToA, "SendEventsToA", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CollisionBlock::SendEventsToA, "SendEventsToB", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CollisionBlock::SendEventsToA, "SendEventsToSpace", Zilch::PropertyBinding::GetSet);
  }
  ZilchDefineType(CollisionFilter, "CollisionFilter", DCEngineCore, builder, type) {

    ZilchBindField(builder, type, &CollisionFilter::CollisionFlag, "CollisionFlag", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CollisionFilter::CollisionStartBlock, "CollisionStartBlock", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CollisionFilter::CollisionEndBlock, "CollisionEndBlock", Zilch::PropertyBinding::GetSet);
    ZilchBindField(builder, type, &CollisionFilter::PreSolveBlock, "PreSolveBlock", Zilch::PropertyBinding::GetSet);
    // the pair of groups
    std::pair<std::string, std::string> Pairing;
  }
  ZilchDefineType(CollisionTable, "CollisionTable", DCEngineCore, builder, type) {
    ZilchBindMethod(builder, type, &CollisionTable::AddGroup, (bool(CollisionTable::*)(CollisionGroup)), "AddGroup", "group");
    ZilchBindMethod(builder, type, &CollisionTable::AddGroup, (bool(CollisionTable::*)(std::string)), "AddGroup", "group");
    //ZilchBindMethod(builder, type, &CollisionTable::GetGroups, ZilchNoOverload, "GetGroups", ZilchNoNames);
    //ZilchBindMethod(builder, type, &CollisionTable::GetPairs, ZilchNoOverload, "GetPairs", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionTable::ScanForGroups, ZilchNoOverload, "ScanForGroups", ZilchNoNames);
    ZilchBindMethod(builder, type, &CollisionTable::GetFilter, ZilchNoOverload, "GetFilter", "group1, group2");
    ZilchBindMethod(builder, type, &CollisionTable::SetResolve, ZilchNoOverload, "SetResolve", "group1, group2, state");
    ZilchBindMethod(builder, type, &CollisionTable::GetStartBlock, ZilchNoOverload, "GetStartBlock", "group1, group2");
    ZilchBindMethod(builder, type, &CollisionTable::SetStartBlock, ZilchNoOverload, "SetStartBlock", "group1, group2, state");
    ZilchBindMethod(builder, type, &CollisionTable::GetEndBlock, ZilchNoOverload, "GetEndBlock", "group1, group2");
    ZilchBindMethod(builder, type, &CollisionTable::SetEndBlock, ZilchNoOverload, "SetEndBlock", "group1, group2, state");
    ZilchBindMethod(builder, type, &CollisionTable::GetPreSolveBlock, ZilchNoOverload, "GetPreSolveBlock", "group1, group2");
    ZilchBindMethod(builder, type, &CollisionTable::SetPreSolveBlock, ZilchNoOverload, "SetPreSolveBlock", "group1, group2, state");
  }
#endif

static CollisionTablePtr Find(std::string);
  /**************************************************************************/
  /*!
  @brief CollisionTable constructor.
  */
  /**************************************************************************/
  CollisionTable::CollisionTable(std::string collisionTableFile) :
    Resource("CollisionTable", FileSystem::FileNoExtension(collisionTableFile), collisionTableFile)
  {    
    AddGroup(CollisionGroup("Default"));
    ScanForGroups();
  }

  /**************************************************************************/
  /*!
  @brief CollisionTable constructor.
  @todo  What? Remove it.
  */
  /**************************************************************************/
  CollisionTable::CollisionTable(void) : Resource("CollisionTable", "CollisionTable", "NoFile")
  {
    AddGroup(CollisionGroup("Default"));
    ScanForGroups();
  }


  std::vector<CollisionGroup> &CollisionTable::GetGroups(void)
  {
    return Groups;
  }

  std::vector<CollisionFilter>  & CollisionTable::GetPairs(void)
  {
    return Pairs;
  }

  const CollisionFilter &CollisionTable::GetFilter(std::string const &group1, std::string const &group2)
  {
    for (auto &Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        return Pair;
      }
    }

    ScanForGroups();

    return GetFilter(group1, group2);

    throw DCException(std::string("CollisionTable::GetFilter - Tried to get filter of a paring that doesnt exist") + std::string(" Group1:") + group1 + std::string(" Group2:") + std::string("\n"));
    return Pairs[0];
  }

  bool CollisionTable::AddGroup(CollisionGroup group)
  {
    for (auto& Group : Groups)
    {
      if (Group.Name() == group.Name())
      {
        //DCTrace << "CollisionTable::AddGroup - Tried to add a group that already exists" << group.Name() << "\n";
        return false;
      }
    }
    Groups.push_back(group);


    CollisionFilter filter;
    filter.Pairing.first = group.Name();

    for (auto& Group : Groups)
    {
      filter.Pairing.second = Group.Name();

      Pairs.push_back(filter);
    }

    return true;
  }

  bool CollisionTable::AddGroup(std::string group)
  {
    for (auto& Group : Groups)
    {
      if (Group.Name() == group)
      {
        //DCTrace << "CollisionTable::AddGroup - Tried to add a group that already exists" << group.Name() << "\n";
        return false;
      }
    }
    Groups.push_back(group);


    CollisionFilter filter;
    filter.Pairing.first = group;

    for (auto& Group : Groups)
    {
      filter.Pairing.second = Group.Name();

      Pairs.push_back(filter);
    }

    return true;
  }

  bool CollisionTable::SetResolve(std::string const &group1, std::string const &group2, CollisionFlag state)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        Pair.CollisionFlag = state;
        return true;
      }
    }


    //DCTrace << "CollisionTable::SetResolve - Tried to change resolution of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  const CollisionFlag &CollisionTable::GetResolve(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        return Pair.CollisionFlag;
      }
    }   

    ScanForGroups();

    return GetResolve(group1, group2);


    throw DCException(std::string("CollisionTable::GetResolve - Tried to get resolution of a paring that doesnt exist") + std::string(" Group1:") + group1 + std::string(" Group2:") + std::string("\n"));
    
  }

  bool CollisionTable::SetStartBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        Pair.CollisionStartBlock = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetStartBlock - Tried to change start block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  const CollisionBlock &CollisionTable::GetStartBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        return Pair.CollisionStartBlock;
      }
    }

    ScanForGroups();

    return GetStartBlock(group1, group2);

    throw DCException(std::string("CollisionTable::GetStartBlock - Tried to get start block of a paring that doesnt exist") + std::string(" Group1:") + group1 + std::string(" Group2:") + std::string("\n"));

  }

  bool CollisionTable::SetEndBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        Pair.CollisionEndBlock = state;
        return true;
      }
    }

    DCTrace << "CollisionTable::SetEndBlock - Tried to change end block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  const CollisionBlock &CollisionTable::GetEndBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        return Pair.CollisionEndBlock;
      }
    }

    ScanForGroups();

    return GetEndBlock(group1, group2);

    throw DCException(std::string("CollisionTable::GetEndBlock - Tried to get end block of a paring that doesnt exist") + std::string(" Group1:") + group1 + std::string(" Group2:") + std::string("\n"));

  }

  bool CollisionTable::SetPreSolveBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        Pair.PreSolveBlock = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetPreSolveBlock - Tried to change presolve block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  const CollisionBlock &CollisionTable::GetPreSolveBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
		if ((Pair.Pairing.first == group1 && Pair.Pairing.second == group2) || (Pair.Pairing.first == group2 && Pair.Pairing.second == group1))
      {
        return Pair.PreSolveBlock;
      }
    }

    ScanForGroups();

    return GetPreSolveBlock(group1, group2);

    throw DCException(std::string("CollisionTable::GetPreSolveBlock - Tried to get end block of a paring that doesnt exist") + std::string(" Group1:") + group1 + std::string(" Group2:") + std::string("\n"));
  }

  /**************************************************************************/
  /*!
  @brief  Returns the texture resource to be used by the graphics system.
  @return A reference to the texture object.
  */
  /**************************************************************************/
  CollisionTablePtr CollisionTable::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getCollisionTable(name);
  }

  CollisionTable &CollisionTable::operator=(const CollisionTable & rhs)
  {
    if (this == &rhs)
    {
      return *this;
    }

    Groups = rhs.Groups;
    Pairs = rhs.Pairs;

    return *this;
  }

  void CollisionTable::ScanForGroups(void)
  {
    DCTrace << "Scanning lol \n";
    auto Groups = Daisy->getSystem<Systems::Content>()->AllCollisionGroups();

    for (auto group : *Groups)
    {
      AddGroup(group.first);
    }
  }

}
