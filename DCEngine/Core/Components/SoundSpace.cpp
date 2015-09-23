#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {

   void SoundSpace::Initialize() {
     Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate); 
     TestMusic();

     // Testing Keyboard

  }

   void SoundSpace::TestMusic() {
     // THIS IS MY JAM
     using namespace Systems;
     std::string myJam = "spacejam.mp3";
     Daisy->getSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
   }

   void SoundSpace::OnLogicUpdate(Events::LogicUpdate* event) {
     //trace << "SoundSpace::OnLogicUpdate \n";
   }

   void SoundSpace::Serialize(Json::Value & root) {
   }

   void SoundSpace::Deserialize(Json::Value & root) {
   }



}