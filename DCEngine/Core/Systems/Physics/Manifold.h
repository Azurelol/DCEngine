/******************************************************************************/
/*!
\file   Manifold.h
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Interface file of the physics Manifold.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
//#include "../../ComponentsInclude.h"
//include "../../Objects/Object.h"
#include "../../Engine/Types.h"

namespace DCEngine
{

  class GameObject;
  class Manifold
  {
  public:
    GameObject* Object1 = NULL;
    GameObject* Object2 = NULL;

    glm::vec3 v1;
    glm::vec3 v2;

    glm::vec3 ContactNormal;
    float Penetration = 0.0f;
    float Restitution = 0.0f;
    float FrictionCof = 0.0f;

    float SeperatingVelocity = 0.0f;
    float ContactImpulse = 0.0f;
    float CalculateSeparatingVelocity();

    bool rigid1 = false;
    bool rigid2 = false;

  };









}