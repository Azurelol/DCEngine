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
    void OnLogicUpdate(Events::LogicUpdate* updateEvent);
    void OnKeyDown(Event* eventPtr);
    void OnKeyUp(Event* eventPtr);
    void OnMouseDown(Event* eventPtr);

    void TestMusic();
    void Update() {}


  private:


  };


}