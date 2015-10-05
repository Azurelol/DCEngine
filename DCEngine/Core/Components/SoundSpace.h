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
<<<<<<< .merge_file_a22188
    
    void PlayCue(std::string soundCueName);
    void PlayCueAt(std::string soundCueName);
    
    void OnLogicUpdate(Events::LogicUpdate* event);
    void Update() {}
    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner) {}
=======

    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner) {}    
>>>>>>> .merge_file_a02692
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

<<<<<<< .merge_file_a22188
=======
    void PlayCue();
    void PlayCueAt();
    
    void OnLogicUpdate(Events::LogicUpdate* event);




    void Update() {}


>>>>>>> .merge_file_a02692
  private:
    void TestMusic();

  };


}