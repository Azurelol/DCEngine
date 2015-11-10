/*****************************************************************************/
/*!
\file   Resolution.h
\author Blaine Reiner
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  A static class containing collision functions.
*/
/******************************************************************************/
#pragma once

#include "Manifold.h"
#include <vector>


namespace DCEngine
{
  class GameObject;
  class Manifold;
  class Collision
  {
  public:
    static bool BoxtoBox(GameObject *obj1, GameObject *obj2, Manifold &result);
    static bool CircletoBox(GameObject * objrect, GameObject * objcircle, Manifold &result);
    static std::vector<std::vector<float>> ProjectOnTo(std::vector<glm::vec3> &verts1, std::vector<glm::vec3> &verts2, glm::vec3 &Axis);
    static bool BoxtoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);
    static bool CircletoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);
    static bool PointToCircle(GameObject *circle, glm::vec3 point);
    static bool PointToRectangle(GameObject *rect, glm::vec3 point);
  };

}

//#endif