#include "Orientation.h"
#include "EngineReference.h"

namespace DCEngine 
{

  float Orientation::GetVectorAngle(Vec3 vector)
  {
    return glm::atan(vector.y / vector.x);
  }

  void Orientation::LookAtPoint(Vec3 point)
  {
    Vec3 Translation = this->Owner()->getComponent<Transform>()->Translation;

    Vec3 temp = glm::normalize(point - Translation);

    this->Owner()->getComponent<Transform>()->Rotation.z = GetVectorAngle(temp);

    WorldForward.x = WorldForward.x;
  }

  void Orientation::LookAtPointWithUp(Vec3 point)
  {
    Vec3 Translation = this->Owner()->getComponent<Transform>()->Translation;

    Vec3 temp = glm::normalize(point - Translation);

    this->Owner()->getComponent<Transform>()->Rotation.z = glm::acos(temp.x) + glm::half_pi<float>();
  }

  void Orientation::LookAtDirection(Vec3 direction)
  {


  }

  void Orientation::LookAtDirectionWithUp(Vec3 direction)
  {

  }
  
  void Orientation::LookAtUp(void)
  {

  }



  Vec3 Orientation::GetLookAtPointRotation(void)
  {
    return Vec3(0, 0, 0);
  }

  Vec3 Orientation::GetLookAtDirectionRotation(void)
  {
    return Vec3(0, 0, 0);
  }
  Vec3 Orientation::GetLookAtPointWithUpRotation(void)
  {
    return Vec3(0, 0, 0);
  }

  Vec3 Orientation::GetLookAtDirectionWithUpRotation(void)
  {
    return Vec3(0, 0, 0);
  }




}