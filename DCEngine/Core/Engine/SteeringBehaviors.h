/*****************************************************************************/
/*!
@file   SteeringBehaviors.h
@author Jason Jorgenson
@par    email: j.jorgenson\@digipen.edu
@date   2/12/2016
@note   This file includes methods for calculating steering forces for 
        autonomous agents
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once

namespace DCEngine
{
  class SteeringBehaviors
  {
  public:
    static Vec3 GetSeekVelocity(Vec3 currentPosition, Vec3 targetPosition, Vec3 currentVelocity, float maxSpeed, float maxForce);
    static Vec3 GetArriveVelocity(Vec3 currentPosition, Vec3 targetPosition, Vec3 currentVelocity, float maxSpeed, float maxForce, float arriveDistance);

  private:
    static Vec3 ClampMagnitude(Vec3 vector, float maxMagnitude);
  };
}