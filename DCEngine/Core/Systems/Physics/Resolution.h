/*****************************************************************************/
/*!
\file   Resolution.h
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics system...
*/
/******************************************************************************/
#pragma once
#include "../System.h"
#include "../../Components/PhysicsSpace.h"
#include "Manifold.h"

namespace DCEngine 
{
  void Resolve(float dt, std::vector<Manifold> &contactlist);
  void ResolveVelocities(float dt, std::vector<Manifold> &contactlist);
  void ResolvePositions(float dt, std::vector<Manifold> &contactlist);
  
}