#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {

  void Transform::Serialize(Json::Value & root) {
  }

  void Transform::Deserialize(Json::Value & root) {
  }

  /**************************************************************************/
  /*!
  @brief Initializes the GameObject.
  @note  The WorldRotation's Z component is set at 0 because ???
  */
  /**************************************************************************/
  void Transform::Initialize() {
    if (TRACE_COMPONENT_INITIALIZE)
      trace << Owner()->Name() << "::" << ObjName << "::Initialize\n";
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

}