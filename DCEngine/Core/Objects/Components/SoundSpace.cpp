#include "SoundSpace.h"

#include "..\Entities\Space.h" // Need access to Space
#include "..\..\Systems\Audio\Audio.h" // PlaySound

namespace DCEngine {

  SoundSpace::SoundSpace(Entity & owner) : Component(EnumeratedComponent::SoundSpace, BitfieldComponent::SoundSpace, owner) {
    if (TRACE_CONSTRUCTOR)
      trace << "SoundSpace::SoundSpace - Constructor \n";
  }

  void SoundSpace::Initialize() {

    // THIS IS MY JAM
    Space* spacePtr = (Space*)Owner();
    using namespace Systems;
    std::string myJam = "Sunflower.ogg";
    spacePtr->GetSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);

  }

  



}