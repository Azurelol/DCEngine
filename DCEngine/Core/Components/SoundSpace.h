#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Entity;
  class SoundCue;
 
  class SoundSpace : public Component {
    friend class Space;

  public:

    Real Volume; // Change the volume for all sounds in the space
    Real Pitch; // Pitch scale for all sounds in the space
    bool Pause; // Pause all sounds in the space

    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner) {}    
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

    void PlayCue();
    void PlayCueAt();
    
    void OnLogicUpdate(Events::LogicUpdate* event);




    void Update() {}


  private:
    void TestMusic();

  };


}