#pragma once

#include <cassert>
#include <memory>
#include <vector>
#include <string>

#include "Object.h"
#include "Component.h"

namespace DCEngine {

  class Entity : public Object {
  public:
    Entity(std::string name) {
      _name = name;
    }
    Entity() { _name = "Entity"; }

    void AddComponent(std::shared_ptr<Component> component);
    void RemoveComponent(EnumeratedComponent ec);
    bool HasComponent(EnumeratedComponent ec);

    mask Mask();
    bool CheckMask(mask m);

    EnumeratedComponent GetCollider();

    // Allows access to attached components of the entity.
    #define GET_COMPONENT(type) \ GetComponent<type>(EnumeratedSystem::##type);
    template <typename T> std::shared_ptr<T> GetComponent(EnumeratedComponent ec);

  private:
    std::string _archetypeName;
    int _runtimeId;
    mask _mask = static_cast<int>(BitfieldComponent::Alive);
    //EnumeratedComponent _collider = EnumeratedComponent::None;
   
    ComponentPtr _components[static_cast<int>(EnumeratedComponent::Capacity)];

    

  };

  using EntityPtr = std::shared_ptr<Entity>;
  using EntityVec = std::vector<EntityPtr>;

} // DCEngine