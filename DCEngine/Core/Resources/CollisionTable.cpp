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
*/
/******************************************************************************/

#include "CollisionTable.h"

namespace DCEngine
{
  CollisionTable::CollisionTable(std::string name) : Resource(name)
  {
    Groups.push_back(CollisionGroup("Default"));
  }

  std::vector<CollisionGroup> const &CollisionTable::GetGroups(void) const
  {
    return Groups;
  }

  std::vector<CollisionTable::CollisionFilter> const & CollisionTable::GetPairs(void) const
  {
    return Pairs;
  }

  bool CollisionTable::AddGroup(CollisionGroup group)
  {
    for (auto Group : Groups)
    {
      if (Group.Name() == group.Name())
      {
        DCTrace << "CollisionTable::AddGroup - Tried to add a group that exists" << group.Name() << "\n";
        return false;
      }
    }

    CollisionFilter filter;
    filter.Pairing.first = group.Name();

    for (auto Group : Groups)
    {
      filter.Pairing.second = Group.Name();

      Pairs.push_back(filter);
    }

    return true;
  }

  bool CollisionTable::AddGroup(std::string group)
  {
    for (auto Group : Groups)
    {
      if (Group.Name() == group)
      {
        DCTrace << "CollisionTable::AddGroup - Tried to add a group that exists" << group << "\n";
        return false;
      }
    }

    CollisionFilter filter;
    filter.Pairing.first = group;

    for (auto Group : Groups)
    {
      filter.Pairing.second = Group.Name();

      Pairs.push_back(filter);
    }

    return true;
  }

  bool CollisionTable::SetResolve(std::string const &group1, std::string const &group2, CollisionFlag state)
  {
    for (auto Pair : Pairs)
    {
      if ((Pair.Pairing.first == group1 || Pair.Pairing.first == group2) && (Pair.Pairing.second == group1 || Pair.Pairing.second == group2))
      {
        Pair.CollisionFlag = state;
        return true;
      }
    }


    DCTrace << "CollisionTable::SetResolve - Tried to change resolution of a paring that doesnt exist" << " Group1:" << group1 << " Group2:" << group2 << "\n";
    return false;
  }

  CollisionTable::CollisionFlag &CollisionTable::GetResolve(std::string const &group1, std::string const &group2)
  {
    for (auto Pair : Pairs)
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
    for (auto Pair : Pairs)
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

  CollisionTable::CollisionBlock &CollisionTable::GetStartBlock(std::string const &group1, std::string const &group2)
  {
    for (auto Pair : Pairs)
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
    for (auto Pair : Pairs)
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

  CollisionTable::CollisionBlock &CollisionTable::GetEndBlock(std::string const &group1, std::string const &group2)
  {
    for (auto Pair : Pairs)
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
    for (auto Pair : Pairs)
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

  CollisionTable::CollisionBlock &CollisionTable::GetPreSolveBlock(std::string const &group1, std::string const &group2)
  {
    for (auto Pair : Pairs)
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

}
