/******************************************************************************/
/*!
@file   BoxCollider.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class Physics;
  class BoxCollider : public Component {
  public:
    friend class Physics;

    Real3 Size = Real3(1, 1, 1);
    Real3 Offset = Real3(0, 0, 0);
    Boolean Ghost = false;
    Boolean SendsEvents = true;
    Boolean IsDrawingCollider = false;

    Real3 getSize(void);
    Real3 getOffset(void);
    bool getGhost(void);
    bool getSendsEvents(void);

    void DrawCollider();

    // These should be private!
    BoxCollider(Entity& owner) : Component(std::string("BoxCollider"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void OnLogicUpdateEvent(Events::LogicUpdate* event);
    void OnCollisionStartedEvent(Events::CollisionStarted* event);
    void OnCollisionEndedEvent(Events::CollisionEnded* event);

    Real3 getColliderScale();
  private:
    Transform* TransformComponent;

    /* Properties */
    // CollisionGroup = 


  };

}
