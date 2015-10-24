/******************************************************************************/
/*!
@file   Circle.h
@author Blaine Reiner@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The CircleCollider component gives a physical representation of a Circle
        in world space so objects can experience collisions with one 
        another. This component interacts directly with the physics system.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class CircleCollider : public Component {
  public:
       
    float Radius = 5;
    Real3 Offset = Real3(0, 0, 0);
    Boolean Ghost = false;
    Boolean SendsEvents = true;
    Boolean IsDrawingCollider = false;

    float getRadius(void);
    Real3 getOffset(void);
    bool getGhost(void);
    bool getSendsEvents(void);


    // draw collider is disabled because function only works for boxes right now
    //void DrawCollider();
    
    // These should be private!
    CircleCollider(Entity& owner) : Component(std::string("CircleCollider"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void OnLogicUpdateEvent(Events::LogicUpdate* event);
    void OnCollisionStartedEvent(Events::CollisionStarted* event);
    void OnCollisionEndedEvent(Events::CollisionEnded* event);


  private:
    Transform* TransformComponent;

    /* Properties */
    // CollisionGroup = 


  };

}