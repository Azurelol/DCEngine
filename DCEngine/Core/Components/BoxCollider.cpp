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
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(BoxCollider, "BoxCollider", DCEngineCore, builder, type) {
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(PhysicsMaterial);
      DCE_BINDING_DEFINE_ATTRIBUTE(CollisionGroup);
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(BoxCollider);
      //ZilchBindConstructor(builder, type, BoxCollider, "owner", Entity&);
      //ZilchBindDestructor(builder, type, BoxCollider);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, PhysicsMaterial);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyPhysicsMaterial, attributePhysicsMaterial);
      DCE_BINDING_DEFINE_PROPERTY(BoxCollider, CollisionGroup);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyCollisionGroup, attributeCollisionGroup);
      ZilchBindProperty(builder, type, &BoxCollider::getSize, &BoxCollider::setSize, "Size");
      ZilchBindProperty(builder, type, &BoxCollider::getOffset, &BoxCollider::setOffset, "Offset");
      ZilchBindProperty(builder, type, &BoxCollider::getGhost, &BoxCollider::setGhost, "Ghost");
      ZilchBindProperty(builder, type, &BoxCollider::getSendsEvents, &BoxCollider::setSendsEvents, "SendsEvents");
      ZilchBindProperty(builder, type, &BoxCollider::getIsDrawingCollider, &BoxCollider::setIsDrawingCollider, "IsDrawingCollider");
    }
#endif

    /**************************************************************************/
    /*!
    @brief BoxCollider constructor.
    @param owner A reference to this component's owner.
    */
    /**************************************************************************/
    BoxCollider::BoxCollider(Entity & owner) : Collider(owner, "BoxCollider")//: Component(std::string("BoxCollider"), owner)
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
      auto debugScale = Vec2(getColliderScale().x,
        getColliderScale().y);
      Vec4 temp(1.0f, 0.0f, 0.0f, 1.0f);
      SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(Vec3(TransformComponent->Translation + Offset),
        debugScale.x, //TransformComponent->Scale.x * Size.x,
        debugScale.y, //TransformComponent->Scale.y * Size.y,
        temp); // Red
    }

    /**************************************************************************/
    /*!
    \brief  CollisionEvents
    */
    /**************************************************************************/
    void BoxCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
      // Move the GameObject??
    }

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