/*****************************************************************************/
/*!
@file   SteeringBehaviors.cpp
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   2/12/2016
@note   This file includes methods for calculating steering forces for
autonomous agents
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "SteeringBehaviors.h"

namespace DCEngine
{
  // A useful resource on Steering Behaviors
  // http://natureofcode.com/book/chapter-6-autonomous-agents/

  Vec3 SteeringBehaviors::GetSeekVelocity(Vec3 currentPosition, 
                                          Vec3 targetPosition, 
                                          Vec3 currentVelocity, 
                                          float maxSpeed, 
                                          float maxForce)
  {
    Vec3 seekVelocity = targetPosition - currentPosition;   // Desired velocity
    seekVelocity.z = 0.0f;                                  // Ignore Z
    seekVelocity = glm::normalize(seekVelocity);            // Normalize for direction
    seekVelocity *= maxSpeed;                               // Multiply by speed to get velocity
    seekVelocity -= currentVelocity;                               // Subtract by current velocity to find a force to get us to our desired velocity from our current velocity
    seekVelocity = ClampMagnitude(seekVelocity, maxForce);  // Limit the force my maxForce
    return seekVelocity;
  }

  Vec3 SteeringBehaviors::GetArriveVelocity(Vec3 currentPosition,
                                            Vec3 targetPosition,
                                            Vec3 currentVelocity,
                                            float maxSpeed,
                                            float maxForce,
                                            float arriveDistance)
  {
    Vec3 arriveForce = targetPosition - currentPosition;
    arriveForce.z = 0;
    float distance = glm::length(arriveForce);
    float arrivalSpeed = 0.0;

    if (distance < arriveDistance)
      arrivalSpeed = maxSpeed * distance / arriveDistance;
    else
      arrivalSpeed = maxSpeed;

    arriveForce *= arrivalSpeed;
    arriveForce -= currentVelocity;
    arriveForce = ClampMagnitude(arriveForce, maxSpeed);
    return arriveForce;
  }

  Vec3 SteeringBehaviors::ClampMagnitude(Vec3 vector, float maxMagnitude)
  {
    if (glm::length(vector) > maxMagnitude)
      return glm::normalize(vector) * maxMagnitude;

    return vector;
  }
}