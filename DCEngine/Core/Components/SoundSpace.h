/******************************************************************************/
/*!
@file   SoundSpace.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  The SoundSpace component handles calls for playing sounds and forwards
        them to the Audio system.
*/
/******************************************************************************/
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

    void PlayCue(std::string soundCueName);
    void PlayCueAt(std::string soundCueName);

    void OnLogicUpdate(Events::LogicUpdate* event);
    void Update() {}
    SoundSpace::SoundSpace(Entity & owner) : Component(std::string("SoundSpace"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
    void TestMusic();

  };


}