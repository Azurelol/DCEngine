#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class BoxCollider : public Component {
  public:
       
    Real3 Size = Real3(1, 1, 1);
    Real3 Offset = Real3(0, 0, 0);
    Boolean Ghost = false;
    Boolean SendsEvents = true;
    Boolean IsDrawingCollider = false;

    void OnLogicUpdateEvent(Events::LogicUpdate* event);
    void DrawCollider();


    // These should be private!
    BoxCollider(Entity& owner) : Component(std::string("BoxCollider"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);


  private:
    Transform* TransformComponent;

    /* Properties */
    // CollisionGroup = 


  };

}
