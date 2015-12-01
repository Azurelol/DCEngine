/******************************************************************************/
/*!
\file   Collision.cpp
\author Blaine Reiner
\par    email: blaine.reiner\@digipen.edu
\par    DigiPen login: blaine.reiner
\date   11/20/2015
\brief  Implements the collision detection Algorithms
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Collision.h"

#include "../../Components/PhysicsSpace.h"
#include "../../Components/Transform.h"
#include "../../Components/RigidBody.h"
#include "../../Components/BoxCollider.h"
#include "../../Components/CircleCollider.h"
#include "../../Engine/Engine.h"

namespace DCEngine
{
  // the problem is that the values in min and max are wrong
  float DetermineRestitution(RigidBody  &a, RigidBody &b)
  {
    return 	std::max(a.getRestitution(), b.getRestitution());
  }

  float DetermineFriction(RigidBody &a, RigidBody &b)
  {
    return sqrt(a.getFriction() * b.getFriction());
  }

  // remember to come back and fill these out with manifold data

  /**************************************************************************/
  /*!
  @brief  This function determines if two 'GameObjects' with the 'BoxCollider'
  component are colliding.
  @param  A pointer to the first object.
  @param  A pointer to the second object.
  @return True if a collision was detected, false otherwise.
  */
  /**************************************************************************/
  bool Collision::BoxtoBox(GameObject * obj1, GameObject * obj2, Manifold &result)
  {
    /* christian if you are looking at this im sorry about the math in here */

    /* get the rigidbodies */
    auto rigidbody1 = obj1->getComponent<RigidBody>();
    auto rigidbody2 = obj2->getComponent<RigidBody>();


    if (rigidbody1 == NULL)
    {
      result.rigid1 = false;
    }
    else
    {
      result.rigid1 = true;
    }

    if (rigidbody2 == NULL)
    {
      result.rigid2 = false;
    }
    else
    {
      result.rigid2 = true;
    }

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }
    else if ((result.rigid1 && !result.rigid2) && (obj1->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if ((!result.rigid1 && result.rigid2) && (obj2->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if (result.rigid1 && result.rigid2)
    {
      if ((obj1->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static) && (obj2->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
      {
        return false;
      }
    }

    /* get the colliders */
    auto boxcollider1 = obj1->getComponent<BoxCollider>();
    auto boxcollider2 = obj2->getComponent<BoxCollider>();

    if (boxcollider1 == NULL || boxcollider2 == NULL)
    {
      throw DCException("An object Missing a BoxCollider component got passed to BoxtoBox");
    }

    /* get the transforms */
    auto transform1 = obj1->getComponent<Transform>();
    auto transform2 = obj2->getComponent<Transform>();

    if (transform1 == NULL || transform2 == NULL)
    {
      throw DCException("An object Missing a Transform component got passed to BoxtoBox");
    }


    result.Penetration = 1000.0f;

    glm::vec3 topL1;
    glm::vec3 topR1;
    glm::vec3 botL1;
    glm::vec3 botR1;

    glm::vec3 topL2;
    glm::vec3 topR2;
    glm::vec3 botL2;
    glm::vec3 botR2;


    glm::vec3 Axis1;
    glm::vec3 Axis2;
    glm::vec3 Axis3;
    glm::vec3 Axis4;

    float Height0 = boxcollider1->getColliderScale().y * 2.0f;
    float Height1 = boxcollider2->getColliderScale().y  * 2.0f;

    float Width0 = boxcollider1->getColliderScale().x  * 2.0f;
    float Width1 = boxcollider2->getColliderScale().x  * 2.0f;

    float rot0 = transform1->WorldRotation.z;
    float rot1 = transform2->WorldRotation.z;

    std::vector<glm::vec3> verts1;
    std::vector<glm::vec3> verts2;

    topL1.x = transform1->Translation.x + boxcollider1->getOffset().x + ((0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    topL1.y = transform1->Translation.y + boxcollider1->getOffset().y + ((0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    verts1.push_back(topL1);

    topR1.x = transform1->Translation.x + boxcollider1->getOffset().x + ((0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    topR1.y = transform1->Translation.y + boxcollider1->getOffset().y + ((0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

    verts1.push_back(topR1);

    botL1.x = transform1->Translation.x + boxcollider1->getOffset().x + ((-0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    botL1.y = transform1->Translation.y + boxcollider1->getOffset().y + ((-0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    verts1.push_back(botL1);

    botR1.x = transform1->Translation.x + boxcollider1->getOffset().x + ((-0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    botR1.y = transform1->Translation.y + boxcollider1->getOffset().y + ((-0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

    verts1.push_back(botR1);

    /*
    */
    topL2.x = transform2->Translation.x + boxcollider2->getOffset().x + ((0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    topL2.y = transform2->Translation.y + boxcollider2->getOffset().y + ((0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    verts2.push_back(topL2);

    topR2.x = transform2->Translation.x + boxcollider2->getOffset().x + ((0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    topR2.y = transform2->Translation.y + boxcollider2->getOffset().y + ((0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

    verts2.push_back(topR2);

    botL2.x = transform2->Translation.x + boxcollider2->getOffset().x + ((-0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    botL2.y = transform2->Translation.y + boxcollider2->getOffset().y + ((-0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    verts2.push_back(botL2);

    botR2.x = transform2->Translation.x + boxcollider2->getOffset().x + ((-0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    botR2.y = transform2->Translation.y + boxcollider2->getOffset().y + ((-0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

    verts2.push_back(botR2);

    /* this block of math above calculates the veracities of the rectangles */
    std::vector<glm::vec3> Axes;

    Axes.push_back(glm::normalize(topR1 - topL1));
    Axes.push_back(glm::normalize(topR1 - botR1));

    Axes.push_back(glm::normalize(botL2 - topL2));
    Axes.push_back(glm::normalize(topL2 - topR2));

    /* this ^ calculates the 4 axis I will be using to determine collision */

    std::vector<std::vector<float>> MinMax;

    /* project onto each axis */
    for (int i = 0; i < Axes.size(); ++i)
    {
      MinMax = ProjectOnTo(verts1, verts2, Axes[i]);

      /* check the two non overlap results */
      if (MinMax[0][0] > MinMax[1][1] || MinMax[0][1] < MinMax[1][0])
      {
        return false;
      }
      /* check the series of overlap results */
      else if (MinMax[0][1] > MinMax[1][0] && MinMax[0][0] < MinMax[1][0])
      {
        if ((MinMax[0][1] - MinMax[1][0]) < result.Penetration)
        {
          result.Penetration = MinMax[0][1] - MinMax[1][0];
          result.ContactNormal = Axes[i];
        }
      }
      else if (MinMax[1][1] > MinMax[0][0] && MinMax[1][0] < MinMax[0][0])
      {
        if (MinMax[1][1] - MinMax[0][0] <= result.Penetration)
        {
          result.Penetration = MinMax[1][1] - MinMax[0][0];
          result.ContactNormal = Axes[i];
        }
      }
      else if (MinMax[1][0] > MinMax[0][0] && MinMax[1][1] < MinMax[0][1])
      {
        if (MinMax[1][0] - MinMax[0][1] > MinMax[0][1] - MinMax[1][1])
        {
          if (MinMax[0][1] - MinMax[1][0] < result.Penetration)
          {
            result.Penetration = MinMax[0][1] - MinMax[1][0];
            result.ContactNormal = Axes[i];
          }
        }
        else
        {
          if (MinMax[1][1] - MinMax[0][0] < result.Penetration)
          {
            result.Penetration = MinMax[1][1] - MinMax[0][0];
            result.ContactNormal = Axes[i];
          }
        }
      }
      else if (MinMax[0][0] > MinMax[1][0] && MinMax[0][1] < MinMax[1][1])
      {
        if (MinMax[1][1] - MinMax[0][1] > MinMax[0][0] - MinMax[1][0])
        {
          if (MinMax[0][1] - MinMax[1][0] < result.Penetration)
          {
            result.Penetration = MinMax[0][1] - MinMax[1][0];
            result.ContactNormal = Axes[i];
          }
        }
        else
        {
          if (MinMax[1][1] - MinMax[0][0] < result.Penetration)
          {
            result.Penetration = MinMax[1][1] - MinMax[0][0];
            result.ContactNormal = Axes[i];
          }
        }
      }

    }

    /* collision is true calculate collision data */

    if (result.Penetration > 200.0f)
    {
      result.Penetration = 1.0f / 60.0f;
    }

    result.Object1 = obj1;
    result.Object2 = obj2;

    if (result.rigid1 != false && result.rigid2 != false)
    {
      result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
      result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
    }
    else if (result.rigid1 == false && result.rigid2 != false)
    {
      result.FrictionCof = rigidbody2->getFriction();
      result.Restitution = rigidbody2->getRestitution();
    }
    else if (result.rigid1 != false && result.rigid2 == false)
    {
      result.FrictionCof = rigidbody1->getFriction();
      result.Restitution = rigidbody1->getRestitution();
    }
    else if (result.rigid1 == false && result.rigid2 == false)
    {
      result.FrictionCof = 0.0f;
      result.Restitution = 0.0f;
    }

    return true;
    //Check X
    glm::vec3 positionDelta = transform1->Translation - transform2->Translation;
    float xDiff = boxcollider1->getColliderScale().x / 2.0f + boxcollider2->getColliderScale().x / 2.0f - fabs(positionDelta.x);

    //Boxes overlapping on x-axis?
    if (0 < xDiff)
    {
      float yDiff = boxcollider1->getColliderScale().y / 2.0f + boxcollider2->getColliderScale().y / 2.0f - fabs(positionDelta.y);

      //Boxes overlapping on y-axis?
      if (0 < yDiff)
      {
        //Which axis is overlapping less? that is the axis we want
        //to use for the collision.
        if (xDiff < yDiff)
        {
          //X is smallest
          glm::vec3 normal = positionDelta.x < 0 ? glm::vec3(-1, 0, 0) : glm::vec3(1, 0, 0);
          result.Object1 = obj1;
          result.Object2 = obj2;
          result.ContactNormal = normal;
          result.Penetration = xDiff;

          if (result.rigid1 != false && result.rigid2 != false)
          {
            result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
            result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
          }
          else if (result.rigid1 == false && result.rigid2 != false)
          {
            result.FrictionCof = rigidbody2->getFriction();
            result.Restitution = rigidbody2->getRestitution();
          }
          else if (result.rigid1 != false && result.rigid2 == false)
          {
            result.FrictionCof = rigidbody1->getFriction();
            result.Restitution = rigidbody1->getRestitution();
          }
          else if (result.rigid1 == false && result.rigid2 == false)
          {
            result.FrictionCof = 0.0f;
            result.Restitution = 0.0f;
          }

          return true;
        }
        else
        {
          //Y is smallest
          glm::vec3 normal = positionDelta.y < 0 ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0);
          result.Object1 = obj1;
          result.Object2 = obj2;
          result.ContactNormal = normal;
          result.Penetration = yDiff;

          if (result.rigid1 != false && result.rigid2 != false)
          {
            result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
            result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
          }
          else if (result.rigid1 == false && result.rigid2 != false)
          {
            result.FrictionCof = rigidbody2->getFriction();
            result.Restitution = rigidbody2->getRestitution();
          }
          else if (result.rigid1 != false && result.rigid2 == false)
          {
            result.FrictionCof = rigidbody1->getFriction();
            result.Restitution = rigidbody1->getRestitution();
          }
          else if (result.rigid1 == false && result.rigid2 == false)
          {
            result.FrictionCof = 0.0f;
            result.Restitution = 0.0f;
          }
          return true;
        }
      }
    }

    return false;

  }



  /**************************************************************************/
  /*!
  @brief  This function projects an objects verts on to a vector.
  @param vec- the vector we are projecting.
  @param Axis- the axis we are projecting on to.
  @param result- where to store the result.
  */
  /**************************************************************************/
  std::vector<std::vector<float>> Collision::ProjectOnTo(std::vector<glm::vec3> &verts1, std::vector<glm::vec3> &verts2, glm::vec3 &Axis)
  {
    std::vector<std::vector<float>> result;

    std::vector<float> obj1;
    std::vector<float> obj2;

    obj1.push_back(0.0f);
    obj1.push_back(0.0f);

    obj2.push_back(0.0f);
    obj2.push_back(0.0f);


    for (int i = 0; i < verts1.size(); ++i)
    {
      if (i == 0)
      {
        obj1[0] = glm::dot(verts1[i], Axis);
        obj1[1] = glm::dot(verts1[i], Axis);
      }
      else
      {
        if (glm::dot(verts1[i], Axis) < obj1[0])
        {
          obj1[0] = glm::dot(verts1[i], Axis);
        }

        if (glm::dot(verts1[i], Axis) > obj1[1])
        {
          obj1[1] = glm::dot(verts1[i], Axis);
        }
      }
    }

    for (int i = 0; i < verts2.size(); ++i)
    {
      if (i == 0)
      {
        obj2[0] = glm::dot(verts2[i], Axis);
        obj2[1] = glm::dot(verts2[i], Axis);
      }
      else
      {
        if (glm::dot(verts2[i], Axis) < obj2[0])
        {
          obj2[0] = glm::dot(verts2[i], Axis);
        }

        if (glm::dot(verts2[i], Axis) > obj2[1])
        {
          obj2[1] = glm::dot(verts2[i], Axis);
        }
      }
    }


    result.push_back(obj1);
    result.push_back(obj2);

    return result;
  }

  /**************************************************************************/
  /*!
  @brief  This function determines if two 'GameObjects', one with a
  'BoxCollider' component and the other with a 'SphereCollider'
  component are colliding.
  @param  A pointer to the first object with the 'BoxCollider'.
  @param  A pointer to the second object with the 'SphereCollider'.
  @return True if a collision was detected, false otherwise.
  */
  /**************************************************************************/
  bool Collision::CircletoBox(GameObject * objrect, GameObject * objcircle, Manifold &result)
  {

    /* get the rigidbodies */
    auto rigidbody1 = objrect->getComponent<RigidBody>();
    auto rigidbody2 = objcircle->getComponent<RigidBody>();

    if (rigidbody1 == NULL)
    {
      result.rigid1 = false;
    }
    else
    {
      result.rigid1 = true;
    }

    if (rigidbody2 == NULL)
    {
      result.rigid2 = false;
    }
    else
    {
      result.rigid2 = true;
    }

    /* get the colliders */
    auto boxcollider = objrect->getComponent<BoxCollider>();
    auto circlecollider = objcircle->getComponent<CircleCollider>();

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }
    else if ((result.rigid1 && !result.rigid2) && (objrect->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if ((!result.rigid1 && result.rigid2) && (objcircle->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if (result.rigid1 && result.rigid2)
    {
      if ((objrect->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static) && (objcircle->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
      {
        return false;
      }
    }

    if (boxcollider == NULL)
    {
      throw DCException("An object Missing a BoxCollider component got passed to BoxtoCircle");
    }

    if (circlecollider == NULL)
    {
      throw DCException("An object Missing a CircleCollider component got passed to BoxtoCircle");
    }

    /* get the transforms */
    auto transform1 = objrect->getComponent<Transform>();
    auto transform2 = objcircle->getComponent<Transform>();

    if (transform1 == NULL || transform2 == NULL)
    {
      throw DCException("An object Missing a Transform component got passed to BoxtoCircle");
    }


    glm::vec3 CircleCenter = transform1->Translation + circlecollider->getOffset();

    CircleCenter -= transform2->Translation + boxcollider->getOffset();

    glm::vec3 temp = CircleCenter;

    CircleCenter.x = cos(-transform2->WorldRotation.z) * temp.x + -sin(-transform2->WorldRotation.z) * temp.y;
    CircleCenter.y = sin(-transform2->WorldRotation.z) * temp.x + cos(-transform2->WorldRotation.z) * temp.y;

    CircleCenter += transform2->Translation + boxcollider->getOffset().x;

    float left = transform2->Translation.x + boxcollider->getOffset().x - (boxcollider->getColliderScale().x / 2.0f);
    float right = transform2->Translation.x + boxcollider->getOffset().x + (boxcollider->getColliderScale().x / 2.0f);
    float top = transform2->Translation.y + boxcollider->getOffset().y + (boxcollider->getColliderScale().y / 2.0f);
    float bottom = transform2->Translation.y + boxcollider->getOffset().y - (boxcollider->getColliderScale().y / 2.0f);

    glm::vec3 vecPoint;

    if (CircleCenter.x > right)
    {
      if (CircleCenter.y > top)
      {
        vecPoint.x = right;
        vecPoint.y = top;
      }
      else if (CircleCenter.y < bottom)
      {
        vecPoint.x = right;
        vecPoint.y = bottom;
      }
      else
      {
        vecPoint.x = right;
        vecPoint.y = CircleCenter.y;
      }

    }
    else if (CircleCenter.x < left)
    {
      if (CircleCenter.y > top)
      {
        vecPoint.x = left;
        vecPoint.y = top;
      }
      else if (CircleCenter.y < bottom)
      {
        vecPoint.x = left;
        vecPoint.y = bottom;
      }
      else
      {
        vecPoint.x = left;
        vecPoint.y = CircleCenter.y;
      }
    }
    else
    {
      if (CircleCenter.y > top)
      {
        vecPoint.x = CircleCenter.x;
        vecPoint.y = top;
      }
      else if (CircleCenter.y < bottom)
      {
        vecPoint.x = CircleCenter.x;
        vecPoint.y = bottom;
      }
      else
      {
        return true;
      }
    }


    if (vecPoint.y > top)
    {
      return 0;
    }

    if (vecPoint.y < bottom)
    {
      return 0;
    }

    if (vecPoint.x > right)
    {
      return 0;
    }

    if (vecPoint.x < left)
    {
      return 0;
    }

    float radius = circlecollider->getRadius();
    glm::vec3 p = CircleCenter;
    glm::vec3 minV(transform1->Translation.x - boxcollider->getSize().x, transform1->Translation.y - boxcollider->getSize().y, 0);
    glm::vec3 maxV(transform1->Translation.x + boxcollider->getSize().x, transform1->Translation.y + boxcollider->getSize().y, 0);

    glm::vec3 q;
    for (int i = 0; i < 2; ++i)
    {
      float v = p[i];
      if (v < minV[i]) v = minV[i];
      if (v > maxV[i]) v = maxV[i];
      q[i] = v;
    }

    glm::vec3 delta = p - q;
    float disSq = glm::length(delta);
    if (disSq < radius)
    {
      if (disSq == 0.0f)
      {
        //internal
        glm::vec3 bdelta = transform2->Translation - transform1->Translation;

        float xd = radius + boxcollider->getSize().x - fabs(bdelta.x);
        float yd = radius + boxcollider->getSize().y - fabs(bdelta.y);

        glm::vec3 normal;
        float penetration;

        if (xd < yd)
        {
          normal = bdelta.x < 0 ? glm::vec3(-1, 0, 0) : glm::vec3(1, 0, 0);
          penetration = xd;
        }
        else
        {
          normal = bdelta.y < 0 ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0);
          penetration = yd;
        }

        glm::vec3 temp = normal;

        normal.x = cos(transform2->WorldRotation.z) * temp.x - sin(transform2->WorldRotation.z) * temp.y;
        normal.y = sin(transform2->WorldRotation.z) * temp.x + cos(transform2->WorldRotation.z) * temp.y;


        result.Object1 = objrect;
        result.Object2 = objcircle;
        result.ContactNormal = normal;
        result.Penetration = penetration;

        if (result.rigid1 != false && result.rigid2 != false)
        {
          result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
          result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
        }
        else if (result.rigid1 == false && result.rigid2 != false)
        {
          result.FrictionCof = rigidbody2->getFriction();
          result.Restitution = rigidbody2->getRestitution();
        }
        else if (result.rigid1 != false && result.rigid2 == false)
        {
          result.FrictionCof = rigidbody1->getFriction();
          result.Restitution = rigidbody1->getRestitution();
        }
        else if (result.rigid1 == false && result.rigid2 == false)
        {
          result.FrictionCof = 0.0f;
          result.Restitution = 0.0f;
        }

      }
      else
      {
        float dis = glm::length(delta);
        result.Object1 = objrect;
        result.Object2 = objcircle;
        result.ContactNormal = delta;
        result.Penetration = -(dis - circlecollider->getRadius());

        if (result.rigid1 != false && result.rigid2 != false)
        {
          result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
          result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
        }
        else if (result.rigid1 == false && result.rigid2 != false)
        {
          result.FrictionCof = rigidbody2->getFriction();
          result.Restitution = rigidbody2->getRestitution();
        }
        else if (result.rigid1 != false && result.rigid2 == false)
        {
          result.FrictionCof = rigidbody1->getFriction();
          result.Restitution = rigidbody1->getRestitution();
        }
        else if (result.rigid1 == false && result.rigid2 == false)
        {
          result.FrictionCof = 0.0f;
          result.Restitution = 0.0f;
        }

      }
    }
    return false;
  }

  /**************************************************************************/
  /*!
  @brief  This function determines if two 'GameObjects' with the
  'SphereCollider' component are colliding.
  @param  A pointer to the first object.
  @param  A pointer to the second object.
  @return True if a collision was detected, false otherwise.
  */
  /**************************************************************************/
  bool Collision::CircletoCircle(GameObject * obj1, GameObject * obj2, Manifold &result)
  {
    /* get the rigidbodies */
    auto rigidbody1 = obj1->getComponent<RigidBody>();
    auto rigidbody2 = obj2->getComponent<RigidBody>();

    if (rigidbody1 == NULL)
    {
      result.rigid1 = false;
    }
    else
    {
      result.rigid1 = true;
    }

    if (rigidbody2 == NULL)
    {
      result.rigid2 = false;
    }
    else
    {
      result.rigid2 = true;
    }

    /* get the colliders */
    auto circlecollider1 = obj1->getComponent<CircleCollider>();
    auto circlecollider2 = obj2->getComponent<CircleCollider>();

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }
    else if ((result.rigid1 && !result.rigid2) && (obj1->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if ((!result.rigid1 && result.rigid2) && (obj2->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if (result.rigid1 && result.rigid2)
    {
      if ((obj1->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static) && (obj2->getComponent<RigidBody>()->DynamicState == DynamicStateType::Static))
      {
        return false;
      }
    }

    if (circlecollider1 == NULL || circlecollider2 == NULL)
    {
      throw DCException("An object Missing a CircleCollider component got passed to CircletoCircle");
    }

    /* get the transforms */
    auto transform1 = obj1->getComponent<Transform>();
    auto transform2 = obj2->getComponent<Transform>();

    if (transform1 == NULL || transform2 == NULL)
    {
      throw DCException("An object Missing a Transform component got passed to CircletoCircle");
    }

    glm::vec3 positionDelta = transform1->Translation - transform2->Translation;
    float disSquared = glm::length(positionDelta);
    float combinedRadius = circlecollider1->getRadius() + circlecollider2->getRadius();

    //Is the distance between the two sphere less
    //then the radius?
    if (disSquared < combinedRadius)
    {
      //Get not squared values



      glm::vec3 normal;
      float penetration;
      if (disSquared == 0.0f)
      {
        //Right on top of each other
        penetration = circlecollider1->getRadius();
        normal = glm::vec3(1, 0, 0);
      }
      else
      {
        float dis = glm::length(positionDelta);
        positionDelta = glm::normalize(positionDelta);
        penetration = combinedRadius - dis;
        normal = positionDelta;
      }

      //Add a contact
      result.Object1 = obj1;
      result.Object2 = obj2;
      result.ContactNormal = positionDelta;//A to B
      result.Penetration = penetration;

      if (result.rigid1 != false && result.rigid2 != false)
      {
        result.FrictionCof = DetermineFriction(*rigidbody1, *rigidbody2);
        result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
      }
      else if (result.rigid1 == false && result.rigid2 != false)
      {
        result.FrictionCof = rigidbody2->getFriction();
        result.Restitution = rigidbody2->getRestitution();
      }
      else if (result.rigid1 != false && result.rigid2 == false)
      {
        result.FrictionCof = rigidbody1->getFriction();
        result.Restitution = rigidbody1->getRestitution();
      }
      else if (result.rigid1 == false && result.rigid2 == false)
      {
        result.FrictionCof = 0.0f;
        result.Restitution = 0.0f;
      }

      return true;
    }
    else
      return false;


    return false;

    return true;
  }


  bool Collision::PointToCircle(GameObject * circle, glm::vec3 point)
  {
    CircleCollider *collider = circle->getComponent<CircleCollider>();
    Transform *transform = circle->getComponent<Transform>();

    if (collider == NULL)
    {
      throw DCException("An object Missing a CircleCollider component got passed to PointtoCircle");
    }


    if (glm::distance(transform->Translation, point) > collider->getRadius())
    {
      return false;
    }

    return true;
  }

  bool Collision::PointToRectangle(GameObject * rect, glm::vec3 point)
  {
    BoxCollider *collider = rect->getComponent<BoxCollider>();
    Transform *transform = rect->getComponent<Transform>();

    float height, width;

    if (collider == NULL)
    {
      height = transform->Scale.y;
      width = transform->Scale.x;
    }
    else
    {
      height = collider->getColliderScale().y;
      width = collider->getColliderScale().x;
    }



    point -= transform->Translation;

    glm::vec3 temp = point;

    point.x = cos(-transform->WorldRotation.z) * temp.x + -sin(-transform->WorldRotation.z) * temp.y;
    point.y = sin(-transform->WorldRotation.z) * temp.x + cos(-transform->WorldRotation.z) * temp.y;

    point += transform->Translation;

    float top = transform->Translation.y + 0.5f * height;
    float left = transform->Translation.x - 0.5f * width;
    float right = transform->Translation.x + 0.5f * width;
    float bottom = transform->Translation.y - 0.5f * height;

    if (point.y > top)
    {
      return false;
    }

    if (point.y < bottom)
    {
      return false;
    }

    if (point.x > right)
    {
      return false;
    }

    if (point.x < left)
    {
      return false;
    }

    return true;




    return true;
  }

}

