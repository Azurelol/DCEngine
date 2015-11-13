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
    // This sets the Handle Manager to take raw pointers to this type
    type->HandleManager = ZilchManagerId(Zilch::PointerManager);
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

  }

  /**************************************************************************/
  /*!
  @brief Updates the GameObject's rotation with respect to its origin.
  */
  /**************************************************************************/
  void Transform::UpdateRotation()
  {

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

}