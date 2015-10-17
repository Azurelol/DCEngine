#include "Orientation.h"
#include "EngineReference.h"

namespace DCEngine 
{

  float Orientation::GetVectorAngle(Real3 vector)
  {
    return glm::atan(vector.y / vector.x);
  }

  void Orientation::LookAtPoint(Real3 point)
  {
    Real3 Translation = this->Owner()->getComponent<Transform>()->Translation;

    Real3 temp = glm::normalize(point - Translation);

    this->Owner()->getComponent<Transform>()->Rotation.z = GetVectorAngle(temp);

    WorldForward.x = WorldForward.x;
  }

  void Orientation::LookAtPointWithUp(Real3 point)
  {
    Real3 Translation = this->Owner()->getComponent<Transform>()->Translation;

    Real3 temp = glm::normalize(point - Translation);

    this->Owner()->getComponent<Transform>()->Rotation.z = glm::acos(temp.x) + glm::half_pi<float>();
  }

  void Orientation::LookAtDirection(Real3 direction)
  {


  }

  void Orientation::LookAtDirectionWithUp(Real3 direction)
  {

  }
  
  void Orientation::LookAtUp(void)
  {

  }



  Real3 Orientation::GetLookAtPointRotation(void)
  {
    return Real3(0, 0, 0);
  }

  Real3 Orientation::GetLookAtDirectionRotation(void)
  {
    return Real3(0, 0, 0);
  }
  Real3 Orientation::GetLookAtPointWithUpRotation(void)
  {
    return Real3(0, 0, 0);
  }

  Real3 Orientation::GetLookAtDirectionWithUpRotation(void)
  {
    return Real3(0, 0, 0);
  }




}