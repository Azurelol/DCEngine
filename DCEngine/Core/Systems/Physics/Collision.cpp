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
  bool BoxtoBox(GameObject * obj1, GameObject * obj2, Manifold &result)
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


    float Min1 = 0.0f, Max1 = 0.0f, Min2 = 0.0f, Max2 = 0.0f;


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


    glm::vec3 ProjvertexTL1;
    glm::vec3 ProjvertexTR1;
    glm::vec3 ProjvertexBL1;
    glm::vec3 ProjvertexBR1;

    glm::vec3 ProjvertexTL2;
    glm::vec3 ProjvertexTR2;
    glm::vec3 ProjvertexBL2;
    glm::vec3 ProjvertexBR2;

    float Height0 = boxcollider1->getColliderScale().y;
    float Height1 = boxcollider2->getColliderScale().y;

    float Width0 = boxcollider1->getColliderScale().x;
    float Width1 = boxcollider2->getColliderScale().x;

    float rot0 = transform1->WorldRotation.z;
    float rot1 = transform2->WorldRotation.z;


    topL1.x = transform1->Translation.x + ((0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    topL1.y = transform1->Translation.y + ((0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    topR1.x = transform1->Translation.x + ((0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    topR1.y = transform1->Translation.y + ((0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));

    botL1.x = transform1->Translation.x + ((-0.5f * Height0) * -sin(rot0)) + ((-0.5f * Width0) * cos(rot0));
    botL1.y = transform1->Translation.y + ((-0.5f * Height0) * cos(rot0)) + ((-0.5f * Width0) * sin(rot0));

    botR1.x = transform1->Translation.x + ((-0.5f * Height0) * -sin(rot0)) + ((0.5f * Width0) * cos(rot0));
    botR1.y = transform1->Translation.y + ((-0.5f * Height0) * cos(rot0)) + ((0.5f * Width0) * sin(rot0));
    /*
    */
    topL2.x = transform2->Translation.x + ((0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    topL2.y = transform2->Translation.y + ((0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    topR2.x = transform2->Translation.x + ((0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    topR2.y = transform2->Translation.y + ((0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

    botL2.x = transform2->Translation.x + ((-0.5f * Height1) * -sin(rot1)) + ((-0.5f * Width1) * cos(rot1));
    botL2.y = transform2->Translation.y + ((-0.5f * Height1) * cos(rot1)) + ((-0.5f * Width1) * sin(rot1));

    botR2.x = transform2->Translation.x + ((-0.5f * Height1) * -sin(rot1)) + ((0.5f * Width1) * cos(rot1));
    botR2.y = transform2->Translation.y + ((-0.5f * Height1) * cos(rot1)) + ((0.5f * Width1) * sin(rot1));

    /* this block of math above calculates the veracities of the rectangles */

    Axis1.x = topR1.x - topL1.x;
    Axis1.y = topR1.y - topL1.y;

    Axis2.x = topR1.x - botR1.x;
    Axis2.y = topR1.y - botR1.y;

    Axis3.x = topL2.x - botL2.x;
    Axis3.y = topL2.y - botL2.y;

    Axis4.x = topL2.x - topR2.x;
    Axis4.y = topL2.y - topR2.y;

    /* this ^ calculates the 4 axis I will be using to determine collision */

    ProjectOnTo(topL1, Axis1, ProjvertexTL1);
    ProjectOnTo(topR1, Axis1, ProjvertexTR1);
    ProjectOnTo(botL1, Axis1, ProjvertexBL1);
    ProjectOnTo(botR1, Axis1, ProjvertexBR1);
    /*
    */
    ProjectOnTo(topL2, Axis1, ProjvertexTL2);
    ProjectOnTo(topR2, Axis1, ProjvertexTR2);
    ProjectOnTo(botL2, Axis1, ProjvertexBL2);
    ProjectOnTo(botR2, Axis1, ProjvertexBR2);

    Min1 = glm::dot(topL1, Axis1);
    Max1 = Min1;

    if (glm::dot(topR1, Axis1) > Max1)
    {
      Max1 = glm::dot(topR1, Axis1);
    }
    else if (glm::dot(topR1, Axis1) < Min1)
    {
      Min1 = glm::dot(topR1, Axis1);
    }

    if (glm::dot(botR1, Axis1) > Max1)
    {
      Max1 = glm::dot(botR1, Axis1);
    }
    else if (glm::dot(botR1, Axis1) < Min1)
    {
      Min1 = glm::dot(botR1, Axis1);
    }

    if (glm::dot(botL1, Axis1) > Max1)
    {
      Max1 = glm::dot(botL1, Axis1);
    }
    else if (glm::dot(botL1, Axis1) < Min1)
    {
      Min1 = glm::dot(botL1, Axis1);
    }



    Min2 = glm::dot(topL2, Axis1);
    Max2 = Min2;

    if (glm::dot(topR2, Axis1) > Max2)
    {
      Max2 = glm::dot(topR2, Axis1);
    }
    else if (glm::dot(topR2, Axis1) < Min2)
    {
      Min2 = glm::dot(topR2, Axis1);
    }

    if (glm::dot(botR2, Axis1) > Max2)
    {
      Max2 = glm::dot(botR2, Axis1);
    }
    else if (glm::dot(botR2, Axis1) < Min2)
    {
      Min2 = glm::dot(botR2, Axis1);
    }

    if (glm::dot(botL2, Axis1) > Max2)
    {
      Max2 = glm::dot(botL2, Axis1);
    }
    else if (glm::dot(botL2, Axis1) < Min2)
    {
      Min2 = glm::dot(botL2, Axis1);
    }

    if (Min1 > Max2 || Min2 > Max1)
    {
      return 0;
    }
    else if (Min1 < Max2 && Min2 < Max1)
    {
      if ((Max1 - Min2) < result.Penetration)
      {
        result.Penetration = Max1 - Min2;
        result.ContactNormal = glm::normalize(Axis1);
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Min1) < result.Penetration)
      {
        result.Penetration = Max2 - Min1;
        result.ContactNormal = glm::normalize(Axis1);
      }
    }
    else if (Min2 > Min1 && Max2 < Max1)
    {
      if ((Max1 - Max2) >(Min2 - Min1))
      {
        if ((Max1 - Max2) < result.Penetration)
        {
          result.Penetration = Max1 - Max2;
          result.ContactNormal = glm::normalize(Axis1);
        }
      }
      else
      {
        if ((Min2 - Min1) < result.Penetration)
        {
          result.Penetration = Min2 - Min1;
          result.ContactNormal = glm::normalize(Axis1);
        }
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Max1) >(Min1 - Min2))
      {
        if ((Max2 - Max1) < result.Penetration)
        {
          result.Penetration = Max2 - Max1;
          result.ContactNormal = glm::normalize(Axis1);
        }
      }
      else
      {
        if ((Min1 - Min2) < result.Penetration)
        {
          result.Penetration = Min1 - Min2;
          result.ContactNormal = glm::normalize(Axis1);
        }
      }
    }
    /**********************************************************/
    /**********************************************************/
    /**********************************************************/
    ProjectOnTo(topL1, Axis2, ProjvertexTL1);
    ProjectOnTo(topR1, Axis2, ProjvertexTR1);
    ProjectOnTo(botL1, Axis2, ProjvertexBL1);
    ProjectOnTo(botR1, Axis2, ProjvertexBR1);
    /*
    */
    ProjectOnTo(topL2, Axis2, ProjvertexTL2);
    ProjectOnTo(topR2, Axis2, ProjvertexTR2);
    ProjectOnTo(botL2, Axis2, ProjvertexBL2);
    ProjectOnTo(botR2, Axis2, ProjvertexBR2);

    Min1 = glm::dot(ProjvertexTL1, Axis2);
    Max1 = Min1;

    if (glm::dot(ProjvertexTR1, Axis2) > Max1)
    {
      Max1 = glm::dot(ProjvertexTR1, Axis2);
    }
    else if (glm::dot(ProjvertexTR1, Axis2) < Min1)
    {
      Min1 = glm::dot(ProjvertexTR1, Axis2);
    }

    if (glm::dot(ProjvertexBR1, Axis2) > Max1)
    {
      Max1 = glm::dot(ProjvertexBR1, Axis2);
    }
    else if (glm::dot(ProjvertexBR1, Axis2) < Min1)
    {
      Min1 = glm::dot(ProjvertexBR1, Axis2);
    }

    if (glm::dot(ProjvertexBL1, Axis2) > Max1)
    {
      Max1 = glm::dot(ProjvertexBL1, Axis2);
    }
    else if (glm::dot(ProjvertexBL1, Axis2) < Min1)
    {
      Min1 = glm::dot(ProjvertexBL1, Axis2);
    }



    Min2 = glm::dot(ProjvertexTL2, Axis2);
    Max2 = Min2;

    if (glm::dot(ProjvertexTR2, Axis2) > Max2)
    {
      Max2 = glm::dot(ProjvertexTR2, Axis2);
    }
    else if (glm::dot(ProjvertexTR2, Axis2) < Min2)
    {
      Min2 = glm::dot(ProjvertexTR2, Axis2);
    }

    if (glm::dot(ProjvertexBR2, Axis2) > Max2)
    {
      Max2 = glm::dot(ProjvertexBR2, Axis2);
    }
    else if (glm::dot(ProjvertexBR2, Axis2) < Min2)
    {
      Min2 = glm::dot(ProjvertexBR2, Axis2);
    }

    if (glm::dot(ProjvertexBL2, Axis2) > Max2)
    {
      Max2 = glm::dot(ProjvertexBL2, Axis2);
    }
    else if (glm::dot(ProjvertexBL2, Axis2) < Min2)
    {
      Min2 = glm::dot(ProjvertexBL2, Axis2);
    }


    if (Min1 > Max2 || Min2 > Max1)
    {
      return 0;
    }
    else if (Min1 < Max2 && Min2 < Max1)
    {
      if ((Max1 - Min2) < result.Penetration)
      {
        result.Penetration = Max1 - Min2;
        result.ContactNormal = glm::normalize(Axis2);
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Min1) < result.Penetration)
      {
        result.Penetration = Max2 - Min1;
        result.ContactNormal = glm::normalize(Axis2);
      }
    }
    else if (Min2 > Min1 && Max2 < Max1)
    {
      if ((Max1 - Max2) >(Min2 - Min1))
      {
        if ((Max1 - Max2) < result.Penetration)
        {
          result.Penetration = Max1 - Max2;
          result.ContactNormal = glm::normalize(Axis2);
        }
      }
      else
      {
        if ((Min2 - Min1) < result.Penetration)
        {
          result.Penetration = Min2 - Min1;
          result.ContactNormal = glm::normalize(Axis2);
        }
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Max1) >(Min1 - Min2))
      {
        if ((Max2 - Max1) < result.Penetration)
        {
          result.Penetration = Max2 - Max1;
          result.ContactNormal = glm::normalize(Axis2);
        }
      }
      else
      {
        if ((Min1 - Min2) < result.Penetration)
        {
          result.Penetration = Min1 - Min2;
          result.ContactNormal = glm::normalize(Axis2);
        }
      }
    }
    /**********************************************************/
    /**********************************************************/
    /**********************************************************/

    ProjectOnTo(topL1, Axis3, ProjvertexTL1);
    ProjectOnTo(topR1, Axis3, ProjvertexTR1);
    ProjectOnTo(botL1, Axis3, ProjvertexBL1);
    ProjectOnTo(botR1, Axis3, ProjvertexBR1);
    /*
    */
    ProjectOnTo(topL2, Axis3, ProjvertexTL2);
    ProjectOnTo(topR2, Axis3, ProjvertexTR2);
    ProjectOnTo(botL2, Axis3, ProjvertexBL2);
    ProjectOnTo(botR2, Axis3, ProjvertexBR2);


    Min1 = glm::dot(ProjvertexTL1, Axis3);
    Max1 = Min1;

    if (glm::dot(ProjvertexTR1, Axis3) > Max1)
    {
      Max1 = glm::dot(ProjvertexTR1, Axis3);
    }
    else if (glm::dot(ProjvertexTR1, Axis3) < Min1)
    {
      Min1 = glm::dot(ProjvertexTR1, Axis3);
    }

    if (glm::dot(ProjvertexBR1, Axis3) > Max1)
    {
      Max1 = glm::dot(ProjvertexBR1, Axis3);
    }
    else if (glm::dot(ProjvertexBR1, Axis3) < Min1)
    {
      Min1 = glm::dot(ProjvertexBR1, Axis3);
    }

    if (glm::dot(ProjvertexBL1, Axis3) > Max1)
    {
      Max1 = glm::dot(ProjvertexBL1, Axis3);
    }
    else if (glm::dot(ProjvertexBL1, Axis3) < Min1)
    {
      Min1 = glm::dot(ProjvertexBL1, Axis3);
    }



    Min2 = glm::dot(ProjvertexTL2, Axis3);
    Max2 = Min2;

    if (glm::dot(ProjvertexTR2, Axis3) > Max2)
    {
      Max2 = glm::dot(ProjvertexTR2, Axis3);
    }
    else if (glm::dot(ProjvertexTR2, Axis3) < Min2)
    {
      Min2 = glm::dot(ProjvertexTR2, Axis3);
    }

    if (glm::dot(ProjvertexBR2, Axis3) > Max2)
    {
      Max2 = glm::dot(ProjvertexBR2, Axis3);
    }
    else if (glm::dot(ProjvertexBR2, Axis3) < Min2)
    {
      Min2 = glm::dot(ProjvertexBR2, Axis3);
    }

    if (glm::dot(ProjvertexBL2, Axis3) > Max2)
    {
      Max2 = glm::dot(ProjvertexBL2, Axis3);
    }
    else if (glm::dot(ProjvertexBL2, Axis3) < Min2)
    {
      Min2 = glm::dot(ProjvertexBL2, Axis3);
    }

    if (Min1 > Max2 || Min2 > Max1)
    {
      return false;
    }
    else if (Min1 < Max2 && Min2 < Max1)
    {
      if ((Max1 - Min2) < result.Penetration)
      {
        result.Penetration = Max1 - Min2;
        result.ContactNormal = glm::normalize(Axis3);
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Min1) < result.Penetration)
      {
        result.Penetration = Max2 - Min1;
        result.ContactNormal = glm::normalize(Axis3);
      }
    }
    else if (Min2 > Min1 && Max2 < Max1)
    {
      if ((Max1 - Max2) >(Min2 - Min1))
      {
        if ((Max1 - Max2) < result.Penetration)
        {
          result.Penetration = Max1 - Max2;
          result.ContactNormal = glm::normalize(Axis3);
        }
      }
      else
      {
        if ((Min2 - Min1) < result.Penetration)
        {
          result.Penetration = Min2 - Min1;
          result.ContactNormal = glm::normalize(Axis3);
        }
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Max1)  >(Min1 - Min2))
      {
        if ((Max2 - Max1) < result.Penetration)
        {
          result.Penetration = Max2 - Max1;
          result.ContactNormal = glm::normalize(Axis3);
        }
      }
      else
      {
        if ((Min1 - Min2) < result.Penetration)
        {
          result.Penetration = Min1 - Min2;
          result.ContactNormal = glm::normalize(Axis3);
        }
      }
    }
    /**********************************************************/
    /**********************************************************/
    /**********************************************************/

    ProjectOnTo(topL1, Axis4, ProjvertexTL1);
    ProjectOnTo(topR1, Axis4, ProjvertexTR1);
    ProjectOnTo(botL1, Axis4, ProjvertexBL1);
    ProjectOnTo(botR1, Axis4, ProjvertexBR1);
    /*
    */
    ProjectOnTo(topL2, Axis4, ProjvertexTL2);
    ProjectOnTo(topR2, Axis4, ProjvertexTR2);
    ProjectOnTo(botL2, Axis4, ProjvertexBL2);
    ProjectOnTo(botR2, Axis4, ProjvertexBR2);


    Min1 = glm::dot(ProjvertexTL1, Axis4);
    Max1 = Min1;

    if (glm::dot(ProjvertexTR1, Axis4) > Max1)
    {
      Max1 = glm::dot(ProjvertexTR1, Axis4);
    }
    else if (glm::dot(ProjvertexTR1, Axis4) < Min1)
    {
      Min1 = glm::dot(ProjvertexTR1, Axis4);
    }

    if (glm::dot(ProjvertexBR1, Axis4) > Max1)
    {
      Max1 = glm::dot(ProjvertexBR1, Axis4);
    }
    else if (glm::dot(ProjvertexBR1, Axis4) < Min1)
    {
      Min1 = glm::dot(ProjvertexBR1, Axis4);
    }

    if (glm::dot(ProjvertexBL1, Axis4) > Max1)
    {
      Max1 = glm::dot(ProjvertexBL1, Axis4);
    }
    else if (glm::dot(ProjvertexBL1, Axis4) < Min1)
    {
      Min1 = glm::dot(ProjvertexBL1, Axis4);
    }



    Min2 = glm::dot(ProjvertexTL2, Axis4);
    Max2 = Min2;

    if (glm::dot(ProjvertexTR2, Axis4) > Max2)
    {
      Max2 = glm::dot(ProjvertexTR2, Axis4);
    }
    else if (glm::dot(ProjvertexTR2, Axis4) < Min2)
    {
      Min2 = glm::dot(ProjvertexTR2, Axis4);
    }

    if (glm::dot(ProjvertexBR2, Axis4) > Max2)
    {
      Max2 = glm::dot(ProjvertexBR2, Axis4);
    }
    else if (glm::dot(ProjvertexBR2, Axis4) < Min2)
    {
      Min2 = glm::dot(ProjvertexBR2, Axis4);
    }

    if (glm::dot(ProjvertexBL2, Axis4) > Max2)
    {
      Max2 = glm::dot(ProjvertexBL2, Axis4);
    }
    else if (glm::dot(ProjvertexBL2, Axis4) < Min2)
    {
      Min2 = glm::dot(ProjvertexBL2, Axis4);
    }

    if (Min1 > Max2 || Min2 > Max1)
    {
      return false;
    }
    else if (Min1 < Max2 && Min2 < Max1)
    {
      if ((Max1 - Min2) < result.Penetration)
      {
        result.Penetration = Max1 - Min2;
        result.ContactNormal = glm::normalize(Axis4);
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Min1) < result.Penetration)
      {
        result.Penetration = Max2 - Min1;
        result.ContactNormal = glm::normalize(Axis4);
      }
    }
    else if (Min2 > Min1 && Max2 < Max1)
    {
      if ((Max1 - Max2) >(Min2 - Min1))
      {
        if ((Max1 - Max2) < result.Penetration)
        {
          result.Penetration = Max1 - Max2;
          result.ContactNormal = glm::normalize(Axis4);
        }
      }
      else
      {
        if ((Min2 - Min1) < result.Penetration)
        {
          result.Penetration = Min2 - Min1;
          result.ContactNormal = glm::normalize(Axis4);
        }
      }
    }
    else if (Min1 > Min2 && Max1 < Max2)
    {
      if ((Max2 - Max1) >(Min1 - Min2))
      {
        if ((Max2 - Max1) < result.Penetration)
        {
          result.Penetration = Max2 - Max1;
          result.ContactNormal = glm::normalize(Axis4);
        }
      }
      else
      {
        if ((Min1 - Min2) < result.Penetration)
        {
          result.Penetration = Min1 - Min2;
          result.ContactNormal = glm::normalize(Axis4);
        }
      }
    }

    /* collision is true calculate collision data */

    result.Object1 = obj1;
    result.Object2 = obj2;
    result.obj1 = Collider::Rectangle;
    result.obj2 = Collider::Rectangle;


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
          result.obj1 = Collider::Rectangle;
          result.obj2 = Collider::Rectangle;
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
          result.obj1 = Collider::Rectangle;
          result.obj2 = Collider::Rectangle;
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
  @brief  This function projects a vector on to another vector.
  @param vec- the vector we are projecting.
  @param Axis- the axis we are projecting on to.
  @param result- where to store the result.
  */
  /**************************************************************************/
  void ProjectOnTo(glm::vec3 vec, glm::vec3 Axis, glm::vec3 &result)
  {
    result.x = ((vec.x * Axis.x + vec.y * Axis.y) / (glm::dot(Axis, Axis))) * Axis.x;
    result.y = ((vec.x * Axis.x + vec.y * Axis.y) / (glm::dot(Axis, Axis))) * Axis.y;
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
  bool CircletoBox(GameObject * objrect, GameObject * objcircle, Manifold &result)
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


    glm::vec3 CircleCenter = transform1->Translation;

    CircleCenter -= transform2->Translation;

    glm::vec3 temp = CircleCenter;

    CircleCenter.x = cos(-transform2->WorldRotation.z) * temp.x + -sin(-transform2->WorldRotation.z) * temp.y;
    CircleCenter.y = sin(-transform2->WorldRotation.z) * temp.x + cos(-transform2->WorldRotation.z) * temp.y;

    CircleCenter += transform2->Translation;

    float left = transform2->Translation.x - (boxcollider->getColliderScale().x / 2.0f);
    float right = transform2->Translation.x + (boxcollider->getColliderScale().x / 2.0f);
    float top = transform2->Translation.y + (boxcollider->getColliderScale().y / 2.0f);
    float bottom = transform2->Translation.y - (boxcollider->getColliderScale().y / 2.0f);

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
  bool CircletoCircle(GameObject * obj1, GameObject * obj2, Manifold &result)
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

  bool PointtoCircle(GameObject * circle, glm::vec3 point)
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

  bool PointtoRect(GameObject * rect, glm::vec3 point)
  {
    BoxCollider *collider = rect->getComponent<BoxCollider>();
    Transform *transform = rect->getComponent<Transform>();

    if (collider == NULL)
    {
      throw DCException("An object Missing a BoxCollider component got passed to PointtoRect");
    }


    point -= transform->Translation;

    glm::vec3 temp = point;

    point.x = cos(-transform->WorldRotation.z) * temp.x + -sin(-transform->WorldRotation.z) * temp.y;
    point.y = sin(-transform->WorldRotation.z) * temp.x + cos(-transform->WorldRotation.z) * temp.y;

    point += transform->Translation;

    float top = transform->Translation.y + 0.5 * collider->getColliderScale().y;
    float left = transform->Translation.x - 0.5 * collider->getColliderScale().x;
    float right = transform->Translation.x + 0.5 * collider->getColliderScale().x;
    float bottom = transform->Translation.y - 0.5 * collider->getColliderScale().y;

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

