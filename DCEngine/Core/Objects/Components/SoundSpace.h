#pragma once

#include "../Component.h"

namespace DCEngine {

  class Entity;

  class SoundSpace : public Component {
    friend class Space;

  public:
    SoundSpace(Entity& owner);

    void Initialize();
    void Update() {}

  private:


  };


}