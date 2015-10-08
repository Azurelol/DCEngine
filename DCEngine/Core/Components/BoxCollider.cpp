#include "BoxCollider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {

  void BoxCollider::Initialize()
  {
    auto owner = dynamic_cast<GameObject*>(Owner());
    // Store a reference to the Transform Component
    TransformComponent = owner->getComponent<Transform>();
    // Subscribe this physics component to the physics space
    SpaceRef->getComponent<PhysicsSpace>()->AddCollider(owner);
   
    // If this component needs to draw the collider with debug draw,
    // subscribe to LogicUpdate events so it can draw
    if (IsDrawingCollider)
      Connect(SpaceRef, Events::LogicUpdate, BoxCollider::OnLogicUpdateEvent);
    
  }



  void BoxCollider::DrawCollider()
  {
    auto debugScale = Real2(TransformComponent->Scale.x * Size.x,
                            TransformComponent->Scale.y * Size.y);

    SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(TransformComponent->Translation + Offset, 
                                                          debugScale.x, //TransformComponent->Scale.x * Size.x,
                                                          debugScale.y, //TransformComponent->Scale.y * Size.y, 
                                                          Real4(1, 0, 0, 1)); // Red
  }

  void BoxCollider::Serialize(Json::Value & root)
  {
  }

  void BoxCollider::Deserialize(Json::Value & root)
  {
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

  void BoxCollider::OnLogicUpdateEvent(Events::LogicUpdate* event)
  {
    DrawCollider();
  }
  
  /* Getters */
  Real3 BoxCollider::getSize(void)
  {
    return this->Size;
  }

  Real3 BoxCollider::getOffset(void)
  {
    return this->Offset;
  }

  bool BoxCollider::getGhost(void)
  {
    return this->Ghost;
  }

  bool BoxCollider::getSendsEvents(void)
  {
    return this->SendsEvents;
  }

}