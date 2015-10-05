#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  void SoundSpace::Initialize() {
    Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate);
    TestMusic();

    // Register this space to the sound system
    Daisy->getSystem<Systems::Audio>()->Register(*this);
  }

  void SoundSpace::TestMusic() {
    // THIS IS MY JAM
    using namespace Systems;
    std::string myJam = "spacejam.mp3";
    Daisy->getSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
  }

  void SoundSpace::PlayCue(std::string soundCueName)
  {
    Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
  }

  void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
    //trace << "SoundSpace::OnLogicUpdate \n";
  }

  void SoundSpace::Serialize(Json::Value & root) {
  }

  void SoundSpace::Deserialize(Json::Value & root) {
  }



}