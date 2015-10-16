#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class GravityEffect : public Component {
  public:

    Boolean Active = true;
    Boolean WakeUpOnChange;
    Boolean LocalSpaceDirection;
    Real Strength = 1;
    Real3 Direction = Real3(0, -1, 0);

    /* Initialize */
    GravityEffect(Entity& owner) : Component(std::string("GravityEffect"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);


    void Update(float dt);

  private:
  };
}