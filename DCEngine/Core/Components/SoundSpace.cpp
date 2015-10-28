#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Initializes the 'SoundSpace' component.
  */
  /**************************************************************************/
  void SoundSpace::Initialize() {
    Connect(SpaceRef, Events::LogicUpdate, SoundSpace::OnLogicUpdate);
    TestMusic();

    // Register this space to the sound system
    Daisy->getSystem<Systems::Audio>()->Register(*this);
  }

  void SoundSpace::Serialize(Json::Value & root) {
  }

  void SoundSpace::Deserialize(Json::Value & root) {
  }

  void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
    //trace << "SoundSpace::OnLogicUpdate \n";
  }

  /**************************************************************************/
  /*!
  @brief  Plays a 'SoundCue', forwarding the data to the Audio system.
  @param  soundCueName The name of the 'SoundCue' to play.
  */
  /**************************************************************************/
  void SoundSpace::PlayCue(std::string soundCueName)
  {
    Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
  }

  /**************************************************************************/
  /*!
  @brief  Stops a 'SoundCue' from playing.
  @param  soundCueName The name of the 'SoundCue' to stop.
  */
  /**************************************************************************/
  void SoundSpace::StopCue(std::string soundCueName)
  {
    Daisy->getSystem<Systems::Audio>()->StopSound(soundCueName);
  }


  /* Testers */

  void SoundSpace::TestMusic() {
    // THIS IS MY JAM
    //using namespace Systems;
    //std::string myJam = "spacejam.mp3";
    //Daisy->getSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
  }

}