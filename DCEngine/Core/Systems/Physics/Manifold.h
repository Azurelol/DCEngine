#pragma once
#include "../../ComponentsInclude.h"
#include "../../Objects/Object.h"

namespace DCEngine
{
  enum Collider
  {
    Circle = 0,
    Rectangle
  };


  class Manifold
  {
  public:

    bool rigid1;
    bool rigid2;

    Vec3 CollisionCenter;
    GameObject* Object1;
    GameObject* Object2;

    Collider obj1;
    Collider obj2;

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