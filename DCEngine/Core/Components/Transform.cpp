/******************************************************************************/
/*!
@file   Transform.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/05/2015
@brief  The transform component allows the representation of this object in
world space, allowing it to be drawn, take part in collisions and
force-based movement through the addition of other components.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Transform.h"
#include "EngineReference.h"

namespace DCEngine {
  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    ZilchDefineType(Transform, "Transform", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(Transform);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(Transform, Translation);
      DCE_BINDING_DEFINE_PROPERTY(Transform, LocalTranslation);
      DCE_BINDING_DEFINE_PROPERTY(Transform, WorldTranslation);
      DCE_BINDING_DEFINE_PROPERTY(Transform, Rotation);
      DCE_BINDING_DEFINE_PROPERTY(Transform, Scale);
    }

    /**************************************************************************/
    /*!
    @brief Initializes the GameObject.
    @note  The WorldRotation's Z component is set at 0 because ???
    */
    /**************************************************************************/
    TransformDataPair Transform::getTransformDataPair()
    {
      TransformDataPair data;
      data.first = this;
      data.second.Translation = Translation;
      data.second.Rotation = Rotation;
      data.second.Scale = Scale;
      return data;
    }

    Transform::Transform(Entity & owner) : Component("Transform", owner),
      Translation(Vec3(0.0f, 0.0f, 0.0f)), Rotation(Vec3(0.0f, 0.0f, 0.0f)),
      Scale(Vec3(1.0f, 1.0f, 1.0f))
    {
    }

    /**************************************************************************/
    /*!
    @brief Initializes the GameObject.
    @note  The WorldRotation's Z component is set at 0 because ???
    */
    /**************************************************************************/
    void Transform::Initialize() {
      if (DCE_TRACE_COMPONENT_INITIALIZE)
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
        PrevTranslation = Translation;
        return;
      }

      auto physpace = this->SpaceRef->getComponent<Components::PhysicsSpace>();

      if (Translation.x > physpace->MaxX)
      {
        physpace->MaxX = Translation.x;
      }

      if (Translation.y > physpace->MaxY)
      {
        physpace->MaxY = Translation.y;
      }

      if (Translation.x < physpace->MinX)
      {
        physpace->MinX = Translation.x;
      }

      if (Translation.y < physpace->MinY)
      {
        physpace->MinY = Translation.y;
      }


      if (dynamic_cast<GameObject*>(this->Owner()))
      {
        auto& children = (dynamic_cast<GameObject*>(this->Owner())->Children());


        if (Translation != PrevTranslation)
        {
          for (auto child : children)
          {
            child->getComponent<Components::Transform>()->Translation += (Translation - PrevTranslation);
            child->getComponent<Components::Transform>()->UpdateTranslation();
          }
        }

        PrevTranslation = Translation;
      }
    }


    /**************************************************************************/
    /*!
    @brief set's the translation with respect to the parent.
    */
    /**************************************************************************/
    void Transform::SetLocalTranslation(const Vec3 & pos)
    {
      auto parentref = dynamic_cast<GameObject*>(this->Owner())->Parent();

      if (parentref)
      {
        auto Center = parentref->getComponent<Components::Transform>()->Translation;


        Translation = Center + pos;
        return;
      }


      Translation = pos;

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
        auto& children = (dynamic_cast<GameObject*>(this->Owner())->Children());

        if (Rotation != PrevRotation)
        {
          for (auto child : children)
          {
            Vec3 temp = RotatePoint(child->getComponent<Components::Transform>()->Translation, Translation, (Rotation.z - PrevRotation.z));
            child->getComponent<Components::Transform>()->Translation = temp;
            child->getComponent<Components::Transform>()->Rotation.z += (Rotation.z - PrevRotation.z);
            child->getComponent<Components::Transform>()->UpdateTranslation();
            child->getComponent<Components::Transform>()->UpdateRotation();
          }
        }

      }

	  PrevRotation = Rotation;
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
    Vec3 Transform::RotatePoint(Vec3 point, Vec3 rotation, float angle)
    {
      point -= rotation;
      Vec3 temp = point;

	  angle = (3.14159265359f / 180.0f) * angle;

      point.x = temp.x * cos(angle) + temp.y * -sin(angle);
      point.y = temp.x * sin(angle) + temp.y * cos(angle);

      point += rotation;

      return point;
    }

    const Vec3 & Transform::getLocalTranslation() const
    {
      auto parentref = dynamic_cast<GameObject*>(this->ObjectOwner)->Parent();

      if (parentref)
      {
        return Translation - parentref->getComponent<Components::Transform>()->getTranslation();
      }

      return Translation;
    }
    
    void Transform::setLocalTranslation(const Vec3 & val)
    {
      auto parentref = dynamic_cast<GameObject*>(this->ObjectOwner)->Parent();

      if (parentref)
      {

        Translation = parentref->getComponent<Components::Transform>()->getTranslation() + val;
        return;
      }

      Translation = val;
    }

    const Vec3 & Transform::getTranslation() const
    {
      return Translation;
    }

    void Transform::setTranslation(const Vec3 & val)
    {
      Translation = val;
    }

    const Vec3 & Transform::getWorldTranslation() const
    {
      return Translation;
    }

    void Transform::setWorldTranslation(const Vec3 & val)
    {
      Translation = val;
    }
    /*
    const Vec3 &Transform::getRotation(void)
    {
      DRotation = Rotation * (180.0f / PI);

      return DRotation;
    }
    void Transform::setRotation(Vec3 const & rot)
    {
      Rotation = rot * (PI / 180.0f);
    }*/


  }
}