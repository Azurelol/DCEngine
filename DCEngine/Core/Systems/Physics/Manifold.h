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
  enum ColliderType
  {
    Circle = 0,
    Rectangle
  };

  class GameObject;
  class Manifold
  {
  public:

    bool rigid1;
    bool rigid2;

    GameObject* Object1;
    GameObject* Object2;

    glm::vec3 v1;
    glm::vec3 v2;

    glm::vec3 ContactNormal;
    float Penetration;
    float Restitution;
    float FrictionCof;

    float SeperatingVelocity;
    float ContactImpulse;
    float CalculateSeparatingVelocity();


  };









}