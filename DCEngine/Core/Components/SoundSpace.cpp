#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {

   void SoundSpace::Initialize() {
     Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate); 
     TestMusic();

<<<<<<< .merge_file_a12836
     // Register this space to the sound system
     Daisy->getSystem<Systems::Audio>()->Register(*this);
=======
     // Testing Keyboard

>>>>>>> .merge_file_a16888
  }

   void SoundSpace::TestMusic() {
     // THIS IS MY JAM
     using namespace Systems;
     std::string myJam = "spacejam.mp3";
     Daisy->getSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
   }

<<<<<<< .merge_file_a12836
   void SoundSpace::PlayCue(std::string soundCueName)
   {
     Daisy->getSystem<Systems::Audio>()->PlaySound(soundCueName);
   }

=======
>>>>>>> .merge_file_a16888
   void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
     //trace << "SoundSpace::OnLogicUpdate \n";
   }

   void SoundSpace::Serialize(Json::Value & root) {
   }

   void SoundSpace::Deserialize(Json::Value & root) {
   }



}