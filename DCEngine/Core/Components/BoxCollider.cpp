/******************************************************************************/
/*!
@file   BoxCollider.cpp
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "BoxCollider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {
  namespace Components
  {
     /**************************************************************************/
     /*!
     @brief Provides the definition of this class to Zilch.
     @note This can only go in the translational unit (.cpp)
     */
     /**************************************************************************/
    ZilchDefineType(BoxCollider, "BoxCollider", DCEngineCore, builder, type) {
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(BoxCollider);
      // Resources Properties
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(PhysicsMaterial);
      DCE_BINDING_DEFINE_ATTRIBUTE(CollisionGroup);   
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, CollisionGroup);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyCollisionGroup, attributeCollisionGroup);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, PhysicsMaterial);
      DCE_BINDING_PROPERTY_SET_RESOURCE_ATTRIBUTE(propertyPhysicsMaterial, attributePhysicsMaterial);
      // Other properties
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, Size);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, Offset);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, Ghost);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, SendsEvents);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, IsDrawingCollider);
    }

    /**************************************************************************/
    /*!
    @brief BoxCollider constructor.
    @param owner A reference to this component's owner.
    */
    /**************************************************************************/
    BoxCollider::BoxCollider(Entity & owner) : Collider(owner, "BoxCollider") 
                                               , Size(1,1,1), Offset(0,0,0), Color(1,0,0,1)
    {
      Register();
    }

    /**************************************************************************/
    /*!
    @brief BoxCollider destructor.
    */
    /**************************************************************************/
    BoxCollider::~BoxCollider()
    {
      SpaceRef->getComponent<Components::PhysicsSpace>()->RemoveCollider(this);
    }

    /**************************************************************************/
    /*!
    @brief Initializes the BoxCollider component.
    */
    /**************************************************************************/
    void BoxCollider::Initialize()
    {
    }

    /**************************************************************************/
    /*!
    @brief Registers the component.
    */
    /**************************************************************************/
    void BoxCollider::Register()
    {
      // Store a reference to the Transform Component
      TransformComponent = Owner()->getComponent<Components::Transform>();
      // Subscribe this physics component to the physics space
      SpaceRef->getComponent<Components::PhysicsSpace>()->AddCollider(this);

      // If this component needs to draw the collider with debug draw,
      // subscribe to LogicUpdate events so it can draw
      if (IsDrawingCollider)
        Connect(SpaceRef, Events::LogicUpdate, BoxCollider::OnLogicUpdateEvent);
    }

    /**************************************************************************/
    /*!
    @brief Event received every update.
    */
    /**************************************************************************/
    void BoxCollider::OnLogicUpdateEvent(Events::LogicUpdate* event)
    {
      DrawCollider();
    }

    /**************************************************************************/
    /*!
    @brief Draws a Collider using DebugDraw.
    */
    /**************************************************************************/
    void BoxCollider::DrawCollider()
    {
      auto debugScale = Vec2(getColliderScale().x, getColliderScale().y);
      SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(Vec3(TransformComponent->Translation + Offset),
        debugScale.x, //TransformComponent->Scale.x * Size.x,
        debugScale.y, //TransformComponent->Scale.y * Size.y,
        Color); // Red
    }

    /**************************************************************************/
    /*!
    \brief  CollisionEvents
    */
    /**************************************************************************/
    void BoxCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    /**************************************************************************/
    /*!
    \brief  CollisionEvents
    */
    /**************************************************************************/
    void BoxCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {

    }

    /**************************************************************************/
    /*!
    @brief  The true scale of the collider is the size set times the scale
    of the component.
    @note   So if an object's transform scale was set at 1 while the collider's
    size was set at 2, the collider would be twice the size of the object.
    */
    /**************************************************************************/
    Vec3 BoxCollider::getColliderScale()
    {
      return (Size * TransformComponent->Scale);
    }


  }
}