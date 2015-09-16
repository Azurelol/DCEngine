#include "SoundSpace.h"

#include "..\Entities\Space.h" // Need access to Space
#include "..\..\Systems\Audio\Audio.h" // PlaySound

#include "../../Engine/Engine.h" // TEMPORARY

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy; // TEMPORARY

   void SoundSpace::Initialize() {
     //Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate); 
     TestMusic();
  }

   void SoundSpace::TestMusic() {
     // THIS IS MY JAM
     using namespace Systems;
     std::string myJam = "spacejam.mp3";
     space_->GetSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
   }

   void SoundSpace::OnLogicUpdate(Events::LogicUpdate* updateEvent) {
     trace << "SoundSpace::OnLogicUpdate - Dt: " << updateEvent->Dt << "\n";
   }

  



}