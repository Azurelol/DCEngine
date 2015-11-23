/******************************************************************************/
/*!
@file   CircleCollider.cpp
@author Blaine Reiner@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The CircleCollider component gives a physical representation of a Circle
        in world space so objects can experience collisions with one
        another. This component interacts directly with the physics system.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"
#include "CircleCollider.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Initializes the CircleCollider component.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  void CircleCollider::Initialize()
  {
    auto owner = dynamic_cast<GameObject*>(Owner());
    // Store a reference to the Transform Component
    TransformComponent = owner->getComponent<Transform>();
    // Subscribe this physics component to the physics space
    SpaceRef->getComponent<PhysicsSpace>()->AddCollider(owner);
   
    // If this component needs to draw the collider with debug draw,
    // subscribe to LogicUpdate events so it can draw
    if (IsDrawingCollider)
      Connect(SpaceRef, Events::LogicUpdate, CircleCollider::OnLogicUpdateEvent);
    
  }



  /*void CircleCollider::DrawCollider()
  {
    auto debugScale = Vec2(TransformComponent->Scale.x * Size.x,
                            TransformComponent->Scale.y * Size.y);

    SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(TransformComponent->Translation + Offset, 
                                                          debugScale.x, //TransformComponent->Scale.x * Size.x,
                                                          debugScale.y, //TransformComponent->Scale.y * Size.y, 
                                                          Vec4(1, 0, 0, 1)); // Red
  }*/

  void CircleCollider::Serialize(Json::Value & root)
  {
  }

  void CircleCollider::Deserialize(Json::Value & root)
  {
  }

  /**************************************************************************/
  /*!
  \brief  CollisionEvents
  */
  /**************************************************************************/
  void CircleCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
  {
    // Move the GameObject??
  }

  void CircleCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
  {

  }

  void CircleCollider::OnLogicUpdateEvent(Events::LogicUpdate* event)
  {
    //DrawCollider();
  }
  
  /* Getters */
  String CircleCollider::getCollisionGroup() const
  {
    return CollisionGroup;
  }

  Vec3 CircleCollider::getOffset(void)
  {
    return this->Offset;
  }

  bool CircleCollider::getGhost(void)
  {
    return this->Ghost;
  }

  bool CircleCollider::getSendsEvents(void)
  {
    return this->SendsEvents;
  }

  float CircleCollider::getRadius(void)
  {
    return Radius;
  }

}