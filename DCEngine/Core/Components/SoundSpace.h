#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Entity;
 
  class SoundSpace : public Component {
    friend class Space;

  public:

    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner) {}
    
    void Initialize();
    void OnLogicUpdate(Events::LogicUpdate* event);
    void OnKeyDown(Events::KeyDown* event);
    void OnKeyUp(Events::KeyUp* event);
    void OnMouseDown(Events::MouseDown* event);

    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void Update() {}


  private:
    void TestMusic();

  };


}