#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Transform;
  class BoxCollider : public Component {
  public:
    
    

    BoxCollider(Entity& owner) : Component(std::string("BoxCollider"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);


  private:
    Transform* TransformComponent;

    /* Properties */
    // CollisionGroup = 
    Real3 Size = Real3(0, 0, 0);
    Real3 Offset = Real3(0, 0, 0);
    bool Ghost = false;
    bool SendsEvents = true;

  };

}
