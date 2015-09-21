#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class TimeSpace : public Component {
  public:
    TimeSpace(Entity& owner) : Component(std::string("TimeSpace"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);
    

  };

}
