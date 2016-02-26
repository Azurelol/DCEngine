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
  float DetermineRestitution(Components::RigidBody  &a, Components::RigidBody &b)
  {
    return 	std::min(a.getRestitution(), b.getRestitution());
  }

  float DetermineFriction(float a, float b)
  {
    return sqrt(a * b);
  }

  /**************************************************************************/
  /*!
  @brief  This function determines if a point is within the boundary
          of a cube defined by translation, scale and rotation data.
  @param  point The position of the point.
  @param  translation The center of the cube.
  @param  rotation The rotation of the cube.
  @param  scale The scale of the cube.  
  @return True if it was found within the bounding area, false if not.
  */

  /**************************************************************************/
  bool Collision::PointToBoundingCube(const Vec3 & point, const Vec3 & translation, const Vec3 & rotation, const Vec3 & scale)
  {
    // This function is inherently wrong because it assumes there is only rotation along the z axis
    // the reason for this flaw is that there is no proper way to resolve the way 3D objects are rotated with
    // the way we store rotation. 

    glm::vec3 Point = point;

    Point -= translation;

    glm::vec3 temp = Point;

    Point.x = cos(-rotation.z) * temp.x + -sin(-rotation.z) * temp.y;
    Point.y = sin(-rotation.z) * temp.x +  cos(-rotation.z) * temp.y;

    Point += translation;

    // assumed to be in right handed coordinates so positive Z is towards the front

    float left  =  translation.x - (scale.x / 2.0f);
    float right =  translation.x + (scale.x / 2.0f);
    float top   =  translation.y + (scale.y / 2.0f);
    float bottom = translation.y - (scale.y / 2.0f);
    float back  =  translation.z - (scale.z / 2.0f);
    float front =  translation.z + (scale.z / 2.0f);

    if (Point.x < left)
    {
      return false;
    }

    if (Point.x > right)
    {
      return false;
    }

    if (Point.y < bottom)
    {
      return false;
    }

    if (Point.y > top)
    {
      return false;
    }

    if (Point.z < back)
    {
      return false;
    }

    if (Point.z > front)
    {
      return false;
    }

    return true;
  }

  bool Collision::PointToBoundingSphere(const Vec3& point, const Vec3& translation, const Vec3& scale, const Vec3& rotation)
  {
    // Christian if you are looking through this I question why you send me a rotation
    // assuming scale.x, y, z are the radius

    if (glm::distance(point, translation) > scale.x)
    {
      return false;
    }


    return true;
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
    auto rigidbody1 = obj1->getComponent<Components::RigidBody>();
    auto rigidbody2 = obj2->getComponent<Components::RigidBody>();

    if (rigidbody1 == NULL)
    {
      result.rigid1 = false;
    }
    else
    {
      result.rigid1 = true;

      if (rigidbody1->getDynamicState() == DynamicStateType::Static)
      {
        result.rigid1 = false;
      }
    }

    if (rigidbody2 == NULL)
    {
      result.rigid2 = false;
    }
    else
    {
      result.rigid2 = true;

      if (rigidbody2->getDynamicState() == DynamicStateType::Static)
      {
        result.rigid2 = false;
      }
    }

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }

    /* get the colliders */
    auto boxcollider1 = obj1->getComponent<Components::BoxCollider>();
    auto boxcollider2 = obj2->getComponent<Components::BoxCollider>();

    if (boxcollider1 == NULL || boxcollider2 == NULL)
    {
      throw DCException("An object Missing a BoxCollider component got passed to BoxtoBox");
    }

    /* get the transforms */
    auto transform1 = obj1->getComponent<Components::Transform>();
    auto transform2 = obj2->getComponent<Components::Transform>();

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

    float Height0 = boxcollider1->getColliderScale().y;
    float Height1 = boxcollider2->getColliderScale().y;

    float Width0 = boxcollider1->getColliderScale().x ;
    float Width1 = boxcollider2->getColliderScale().x ;

    float rot0 = transform1->Rotation.z;
    float rot1 = transform2->Rotation.z;

    std::vector<glm::vec3> verts1;
    std::vector<glm::vec3> verts2;

    glm::vec3 Translation1, Translation2;

    if (result.rigid1)
    {
      Translation1 = rigidbody1->Position;
    }
    else
    {
      Translation1 = transform1->Translation;
    }

    if (result.rigid2)
    {
      Translation2 = rigidbody2->Position;
    }
    else
    {
      Translation2 = transform2->Translation;
    }

    topL1.x = Translation1.x + boxcollider1->getOffset().x + ((0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    topL1.y = Translation1.y + boxcollider1->getOffset().y + ((0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    verts1.push_back(topL1);

    topR1.x = Translation1.x + boxcollider1->getOffset().x + ((0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    topR1.y = Translation1.y + boxcollider1->getOffset().y + ((0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

    verts1.push_back(topR1);

    botL1.x = Translation1.x + boxcollider1->getOffset().x + ((-0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    botL1.y = Translation1.y + boxcollider1->getOffset().y + ((-0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    verts1.push_back(botL1);

    botR1.x = Translation1.x + boxcollider1->getOffset().x + ((-0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    botR1.y = Translation1.y + boxcollider1->getOffset().y + ((-0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

    verts1.push_back(botR1);

    /*
    */
    topL2.x = Translation2.x + boxcollider2->getOffset().x + ((0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    topL2.y = Translation2.y + boxcollider2->getOffset().y + ((0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    verts2.push_back(topL2);

    topR2.x = Translation2.x + boxcollider2->getOffset().x + ((0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    topR2.y = Translation2.y + boxcollider2->getOffset().y + ((0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

    verts2.push_back(topR2);

    botL2.x = Translation2.x + boxcollider2->getOffset().x + ((-0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    botL2.y = Translation2.y + boxcollider2->getOffset().y + ((-0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    verts2.push_back(botL2);

    botR2.x = Translation2.x + boxcollider2->getOffset().x + ((-0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    botR2.y = Translation2.y + boxcollider2->getOffset().y + ((-0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

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
      result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
      result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
    }
    else if (result.rigid1 == false && result.rigid2 != false)
    {
      result.FrictionCof = DetermineFriction(0.5f, rigidbody2->getFriction());
      result.Restitution = rigidbody2->getRestitution();
    }
    else if (result.rigid1 != false && result.rigid2 == false)
    {
      result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), 0.5f);
      result.Restitution = rigidbody1->getRestitution();
    }
    else if (result.rigid1 == false && result.rigid2 == false)
    {
      result.FrictionCof = 0.0f;
      result.Restitution = 0.0f;
    }

    return true;
    //Check X
    glm::vec3 positionDelta = Translation1 - Translation2;
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
            result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
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
            result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
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
    auto rigidbody1 = objrect->getComponent<Components::RigidBody>();
    auto rigidbody2 = objcircle->getComponent<Components::RigidBody>();

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
    auto boxcollider = objrect->getComponent<Components::BoxCollider>();
    auto circlecollider = objcircle->getComponent<Components::CircleCollider>();

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }
    else if ((result.rigid1 && !result.rigid2) && (objrect->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if ((!result.rigid1 && result.rigid2) && (objcircle->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if (result.rigid1 && result.rigid2)
    {
      if ((objrect->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static) && (objcircle->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
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
    auto transform1 = objrect->getComponent<Components::Transform>();
    auto transform2 = objcircle->getComponent<Components::Transform>();

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
          result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
          result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
        }
        else if (result.rigid1 == false && result.rigid2 != false)
        {
          result.FrictionCof = DetermineFriction(0.5f, rigidbody2->getFriction());
          result.Restitution = rigidbody2->getRestitution();
        }
        else if (result.rigid1 != false && result.rigid2 == false)
        {
          result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), 0.5f);
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
          result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
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
    auto rigidbody1 = obj1->getComponent<Components::RigidBody>();
    auto rigidbody2 = obj2->getComponent<Components::RigidBody>();

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
    auto circlecollider1 = obj1->getComponent<Components::CircleCollider>();
    auto circlecollider2 = obj2->getComponent<Components::CircleCollider>();

    if (!result.rigid1 && !result.rigid2)
    {
      return false;
    }
    else if ((result.rigid1 && !result.rigid2) && (obj1->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if ((!result.rigid1 && result.rigid2) && (obj2->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
    {
      return false;
    }
    else if (result.rigid1 && result.rigid2)
    {
      if ((obj1->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static) && (obj2->getComponent<Components::RigidBody>()->DynamicState == DynamicStateType::Static))
      {
        return false;
      }
    }

    if (circlecollider1 == NULL || circlecollider2 == NULL)
    {
      throw DCException("An object Missing a CircleCollider component got passed to CircletoCircle");
    }

    /* get the transforms */
    auto transform1 = obj1->getComponent<Components::Transform>();
    auto transform2 = obj2->getComponent<Components::Transform>();

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
        result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), rigidbody2->getFriction());
        result.Restitution = DetermineRestitution(*rigidbody1, *rigidbody2);
      }
      else if (result.rigid1 == false && result.rigid2 != false)
      {
        result.FrictionCof = DetermineFriction(0.5f, rigidbody2->getFriction());
        result.Restitution = rigidbody2->getRestitution();
      }
      else if (result.rigid1 != false && result.rigid2 == false)
      {
        result.FrictionCof = DetermineFriction(rigidbody1->getFriction(), 0.5f);
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
    Components::CircleCollider *collider = circle->getComponent<Components::CircleCollider>();
    Components::Transform *transform = circle->getComponent<Components::Transform>();

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

  
  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @param  
  @param  
  @return
  @todo Look at the comment.
  */
  /**************************************************************************/
  bool Collision::PointToRectangle(GameObject * rect, glm::vec3 point)
  {
    Components::BoxCollider *collider = rect->getComponent<Components::BoxCollider>();
    Components::Transform *transform = rect->getComponent<Components::Transform>();

    if (!transform)
      return false;

    float height, width;

    height = transform->Scale.y / 2;
    width = transform->Scale.x / 2;

   // if (collider == NULL)
   // {
   //   height = transform->Scale.y;
   //   width = transform->Scale.x;
   // }
   // else
   // {
   //   height = collider->getColliderScale().y;
   //   width = collider->getColliderScale().x;
   // }



    point -= transform->Translation;

    glm::vec3 temp = point;

    point.x = cos(-transform->WorldRotation.z) * temp.x + -sin(-transform->WorldRotation.z) * temp.y;
    point.y = sin(-transform->WorldRotation.z) * temp.x + cos(-transform->WorldRotation.z) * temp.y;

    point += transform->Translation;

    float top = transform->Translation.y + height;
    float left = transform->Translation.x - width;
    float right = transform->Translation.x + width;
    float bottom = transform->Translation.y - height;

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


  bool Collision::SelectiontoBox(Vec3& posSel, float w, float h, const Vec3& posItem, const Vec3& scale, float rot)
  {  
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
  
    float Height0 = h;
    float Height1 = scale.y;
  
    float Width0 = w;
    float Width1 = scale.x;
  
    float rot0 = 0.0f;
    float rot1 = rot;
  
    std::vector<glm::vec3> verts1;
    std::vector<glm::vec3> verts2;
  
    topL1.x = posSel.x + ((0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    topL1.y = posSel.y + ((0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));
  
    verts1.push_back(topL1);
  
    topR1.x = posSel.x + ((0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    topR1.y = posSel.y + ((0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));
  
    verts1.push_back(topR1);
  
    botL1.x = posSel.x + ((-0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    botL1.y = posSel.y + ((-0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));
  
    verts1.push_back(botL1);
  
    botR1.x = posSel.x + ((-0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    botR1.y = posSel.y + ((-0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));
  
    verts1.push_back(botR1);
  
    /*
    */
    topL2.x = posItem.x + ((0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    topL2.y = posItem.y + ((0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));
  
    verts2.push_back(topL2);
  
    topR2.x = posItem.x + ((0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    topR2.y = posItem.y + ((0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));
  
    verts2.push_back(topR2);
  
    botL2.x = posItem.x + ((-0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    botL2.y = posItem.y + ((-0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));
  
    verts2.push_back(botL2);
  
    botR2.x = posItem.x + ((-0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    botR2.y = posItem.y + ((-0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));
  
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
    }
  
    /* collision is true */
  
    return true;
  }

  bool Collision::RayToCollider(const Ray line, GameObject *obj, float &Distance)
  {
    auto box = obj->getComponent<Components::BoxCollider>();
    auto circle = obj->getComponent<Components::CircleCollider>();
    auto Translation = obj->getComponent<Components::Transform>();

    if (box)
    {
      Vec3 topL, topR, botL, botR, Rseg;

      bool retval = false;

      float Height = box->getSize().y, Width = box->getSize().x, rot = Translation->Rotation.z;

      float Dist = FLT_MAX;

      topL.x = Translation->Translation.x + box->getOffset().x +  ((0.5f * Height) * -sin(rot)) + ((-0.5f * Width) * cos(rot));
      topL.y = Translation->Translation.y + box->getOffset().y +  ((0.5f * Height) *  cos(rot)) + ((-0.5f * Width) * sin(rot));

      topR.x = Translation->Translation.x + box->getOffset().x +  ((0.5f * Height) * -sin(rot)) +  ((0.5f * Width) * cos(rot));
      topR.y = Translation->Translation.y + box->getOffset().y +  ((0.5f * Height) *  cos(rot)) +  ((0.5f * Width) * sin(rot));

      botL.x = Translation->Translation.x + box->getOffset().x + ((-0.5f * Height) * -sin(rot)) + ((-0.5f * Width) * cos(rot));
      botL.y = Translation->Translation.y + box->getOffset().y + ((-0.5f * Height) *  cos(rot)) + ((-0.5f * Width) * sin(rot));

      botR.x = Translation->Translation.x + box->getOffset().x + ((-0.5f * Height) * -sin(rot)) +  ((0.5f * Width) * cos(rot));
      botR.y = Translation->Translation.y + box->getOffset().y + ((-0.5f * Height) *  cos(rot)) +  ((0.5f * Width) * sin(rot));
      

      if (RayToSegment(line.Origin, line.Direction, topL, topR, Rseg))
      {
        Distance = glm::length(Rseg - line.Origin);
        return true;
      }

      if (RayToSegment(line.Origin, line.Direction, topR, botR, Rseg))
      {
        Distance = glm::length(Rseg - line.Origin);
        return true;
      }

      if (RayToSegment(line.Origin, line.Direction, botR, botL, Rseg))
      {
        Distance = glm::length(Rseg - line.Origin);
        return true;
      }

      if (RayToSegment(line.Origin, line.Direction, botL, topL, Rseg))
      {
        Distance = glm::length(Rseg - line.Origin);
        return true;
      }


      return false;
    }

    if (circle)
    {

    }

    return false;
  }

  bool Collision::RayToSegment(Vec3 O, Vec3 Dir, Vec3 Begin, Vec3 End, Vec3 &Result)
  {
    Vec3 RayEnd = O + Dir * 10000.0f;

    float D = (O.x - RayEnd.x)*(Begin.y - End.y) - (O.y - RayEnd.y)*(Begin.x - End.x);
    
    if (D == 0)
    {
      return false;
    }

    float x = ((Begin.x - End.x) * (O.x * RayEnd.y - O.y * RayEnd.x) - (O.x - RayEnd.x) * (Begin.x * End.y - Begin.y * End.x)) / D;
    float y = ((Begin.y - End.y) * (O.x * RayEnd.y - O.y * RayEnd.x) - (O.y - RayEnd.y) * (Begin.x * End.y - Begin.y * End.x)) / D;


    if (x < std::min(O.x, RayEnd.x) ||  x > std::max(O.x, RayEnd.x) ||
        x < std::min(Begin.x, End.x) || x > std::max(Begin.x, End.x))
    {
      return false;
    }
    if (y < std::min(O.y, RayEnd.y) || y > std::max(O.y, RayEnd.y) ||
        y < std::min(Begin.y, End.y) || y > std::max(Begin.y, End.y))
    {
      return false;
    }


    Result = Vec3(x, y, 0);

    return true;


    /*Dir = glm::normalize(Dir);
    Vec3 SegDir = glm::normalize(End - Begin);
    
    if (Dir == SegDir || Dir == -SegDir)
    {
      return false;
    }

    SegDir = End - Begin;

    float x, y, S, T;

    S = (O.y * Dir.x + Begin.x - O.x - Begin.y * Dir.x) / (SegDir.y * Dir.x - SegDir.x);

    T = (Begin.x + S * SegDir.x - O.x) / Dir.x;

    Vec3 Temp = O + T * Dir;

    Vec3 Tempvec = Temp - Begin;

    if (glm::normalize(SegDir) == glm::normalize(Tempvec))
    {
      Result = Temp;
      return true;
    }

    return false;*/
  }

}