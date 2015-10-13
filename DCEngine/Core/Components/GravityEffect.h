#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class GravityEffect : public Component {
  public:

    Boolean Active;
    Boolean WakeUpOnChange;
    Boolean LocalSpaceDirection;
    Real Strength;
    Real3 Direction;

    /* Initialize */
    GravityEffect(GravityEffect& owner) : Component(std::string("GravityEffect"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
  };
}