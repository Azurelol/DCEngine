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
  ZilchDefineType(CollisionTable, "CollisionTable", DCEngineCore, builder, type) {
  }
#endif

  /**************************************************************************/
  /*!
  @brief CollisionTable constructor.
  */
  /**************************************************************************/
  CollisionTable::CollisionTable(std::string collisionTableFile) :
    Resource("CollisionTable", FileSystem::FileNoExtension(collisionTableFile), collisionTableFile)
  {
    AddGroup(CollisionGroup("Default"));
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
  }


  std::vector<CollisionGroup> const &CollisionTable::GetGroups(void) const
  {
    return Groups;
  }

  std::vector<CollisionFilter> const & CollisionTable::GetPairs(void) const
  {
    return Pairs;
  }

  CollisionFilter &CollisionTable::GetFilter(std::string const &group1, std::string const &group2)
  {
    for (auto &Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        return Pair;
      }
    }


    DCTrace << "CollisionTable::GetFilter - Tried to get filter of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
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
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        Pair.CollisionFlag = state;
        return true;
      }
    }


    //DCTrace << "CollisionTable::SetResolve - Tried to change resolution of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  CollisionFlag &CollisionTable::GetResolve(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        return Pair.CollisionFlag;
      }
    }

    CollisionFlag temp = CollisionFlag::Resolve;

    DCTrace << "CollisionTable::GetResolve - Tried to get resolution of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return temp;
  }

  bool CollisionTable::SetStartBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        Pair.CollisionStartBlock = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetStartBlock - Tried to change start block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  CollisionBlock &CollisionTable::GetStartBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        return Pair.CollisionStartBlock;
      }
    }

    CollisionBlock temp;

    DCTrace << "CollisionTable::GetStartBlock - Tried to get start block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return temp;
  }

  bool CollisionTable::SetEndBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        Pair.CollisionEndBlock = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetEndBlock - Tried to change end block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  CollisionBlock &CollisionTable::GetEndBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        return Pair.CollisionEndBlock;
      }
    }

    CollisionBlock temp;

    DCTrace << "CollisionTable::GetEndBlock - Tried to get end block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return temp;
  }

  bool CollisionTable::SetPreSolveBlock(std::string const &group1, std::string const &group2, CollisionBlock state)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        Pair.PreSolveBlock = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetPreSolveBlock - Tried to change presolve block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  CollisionBlock &CollisionTable::GetPreSolveBlock(std::string const &group1, std::string const &group2)
  {
    for (auto& Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        return Pair.PreSolveBlock;
      }
    }

    CollisionBlock temp;

    DCTrace << "CollisionTable::GetPreSolveBlock - Tried to get end block of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return temp;
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

}
