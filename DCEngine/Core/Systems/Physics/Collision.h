#pragma once
//#ifdef Collision.h
//#define Collision.h
#include "../System.h"
#include "Manifold.h"

namespace DCEngine
{

  bool BoxtoBox(GameObject *obj1, GameObject *obj2, Manifold &result);
  bool CircletoBox(GameObject * objrect, GameObject * objcircle, Manifold &result);
  std::vector<std::vector<float>> ProjectOnTo(std::vector<glm::vec3> &verts1, std::vector<glm::vec3> &verts2, glm::vec3 &Axis);
  bool BoxtoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);
  bool CircletoCircle(GameObject *obj1, GameObject *obj2, Manifold &result);
  bool PointToCircle(GameObject *circle, glm::vec3 point);
  bool PointToRectangle(GameObject *rect, glm::vec3 point);

}

//#endif