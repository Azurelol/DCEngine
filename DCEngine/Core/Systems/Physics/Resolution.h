/******************************************************************************/
/*!
\file   Resolution.h
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Resolves the collisions betwen objects
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