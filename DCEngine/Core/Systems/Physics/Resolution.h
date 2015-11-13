/*****************************************************************************/
/*!
\file   Resolution.h
\author Blaine Reiner
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  A static class containing resolution functions.
*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Components/PhysicsSpace.h"
#include "Manifold.h"

namespace DCEngine 
{
  class Resolution {
  public:
    static void Resolve(float dt, std::vector<Manifold> &contactlist);
    static void ResolveContactVelocity(float dt, Manifold &c);
    static void ResolveVelocities(float dt, std::vector<Manifold> &contactlist);
    static void ResolvePositions(float dt, std::vector<Manifold> &contactlist);
    static void ResolvePenetration(float dt, Manifold& c);
  };

  
  
}