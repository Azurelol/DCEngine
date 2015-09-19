#pragma once
#include "../Component.h"

namespace DCEngine {

  class Entity;

  class SoundSpace : public Component {
    friend class Space;

  public:
    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), 
                                             EnumeratedComponent::SoundSpace, 
                                             BitfieldComponent::SoundSpace, owner) {}


    void Initialize();
    void OnLogicUpdate(Events::LogicUpdate* event);
    void OnKeyDown(Events::KeyDown* event);
    void OnKeyUp(Events::KeyUp* event);
    void OnMouseDown(Events::MouseDown* event);

    void TestMusic();
    void Update() {}


  private:


  };


}