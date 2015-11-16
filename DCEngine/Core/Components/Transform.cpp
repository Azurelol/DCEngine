#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(Transform, "Transform", DCEngineCore, builder, type) {
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, Transform, "owner", Entity&);
    ZilchBindDestructor(builder, type, Transform);
    // Properties
    ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
    ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
    ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
  }
  #endif

  /**************************************************************************/
  /*!
  @brief Initializes the GameObject.
  @note  The WorldRotation's Z component is set at 0 because ???
  */
  /**************************************************************************/
  void Transform::Initialize() {
    if (TRACE_COMPONENT_INITIALIZE)
      DCTrace << Owner()->Name() << "::" << Name() << "::Initialize\n";
    WorldRotation.z = 0;
  }

  /**************************************************************************/
  /*!
  @brief Updates the GameObject's translation with respect to its origin.
  @note The origin is relative to its parent. By default the GameObject is
        parented to the Space. When attached to another GameObject, that
        GameObject's translation becomes its origin.
  */
  /**************************************************************************/
  void Transform::UpdateTranslation()
  {

    if (firstloop)
    {
      PrevRotation = Rotation;
      return;
    }

    if (dynamic_cast<GameObject*>(this->Owner()))
    {
      GameObjectVec &children = (dynamic_cast<GameObject*>(this->Owner())->Children());


      if (Translation != PrevTranslation)
      {
        for (auto child : children)
        {
          child->getComponent<Transform>()->Translation += (Translation - PrevTranslation);
          child->getComponent<Transform>()->UpdateTranslation();
        }
      }

      PrevTranslation = Translation;
    }
  }

  /**************************************************************************/
  /*!
  @brief Updates the GameObject's rotation with respect to its origin.
  */
  /**************************************************************************/
  void Transform::UpdateRotation()
  {
    if (firstloop)
    {
      PrevRotation = Rotation;
      firstloop = false;
      return;
    }
    
    if (dynamic_cast<GameObject*>(this->Owner()))
    {
      GameObjectVec &children = (dynamic_cast<GameObject*>(this->Owner())->Children());

      if (Rotation != PrevRotation)
      {
        for (auto child : children)
        {
          Vec3 temp = RotatePoint(child->getComponent<Transform>()->Translation, Translation, (Rotation.z - PrevRotation.z));
          child->getComponent<Transform>()->Translation = temp;
          child->getComponent<Transform>()->Rotation.z += (Rotation.z - PrevRotation.z);
          child->getComponent<Transform>()->UpdateTranslation();
          child->getComponent<Transform>()->UpdateRotation();
        }
      }

    }
  }

  /**************************************************************************/
  /*!
  @brief Updates the GameObject's scale with respect to its origin.
  */
  /**************************************************************************/
  void Transform::UpdateScale()
  {

  }

  /**************************************************************************/
  /*                          Properties                                    */
  /**************************************************************************/
  Vec3 Transform::getTranslation()
  {
    return Translation;
  }

  void Transform::setTranslation(Vec3 translation)
  {
    Translation = translation;
  }

  Vec3 Transform::getRotation()
  {
    return Rotation;
  }

  void Transform::setRotation(Vec3 rotation)
  {
    Rotation = rotation;
  }

  Vec3 Transform::getScale()
  {
    return Scale;
  }

  void Transform::setScale(Vec3 scale)
  {
    Scale = scale;
  }

  Vec3 Transform::RotatePoint(Vec3 point, Vec3 rotation, float angle)
  {
    point -= rotation;
    Vec3 temp = point;

    point.x = cos(temp.x) + -sin(temp.y);
    point.y = sin(temp.x) + cos(temp.y);

    point += rotation;

    return point;
  }

}