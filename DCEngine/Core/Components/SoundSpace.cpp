#include "SoundSpace.h"
#include "EngineReference.h"

namespace DCEngine {

   void SoundSpace::Initialize() {
     Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate); 
     TestMusic();

     // Testing Keyboard
     Connect(Daisy->getKeyboard(), Events::KeyDown, SoundSpace::OnKeyDown);
     Connect(Daisy->getKeyboard(), Events::KeyUp, SoundSpace::OnKeyUp);
     Connect(Daisy->getMouse(), Events::MouseDown, SoundSpace::OnMouseDown);
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

   void SoundSpace::OnKeyDown(Events::KeyDown * event) {
     trace << "SoundSpace::OnKeyPressed\n";

     if (event->Key == Keys::Up) {
       trace << "'Up' key was pressed!\n";
     }
     else if (event->Key == Keys::Down) {
       trace << "'Down' key was pressed!\n";
     }
     else if (event->Key == Keys::Left) {
       trace << "'Left' key was pressed!\n";
     }
     else if (event->Key == Keys::Right) {
       trace << "'Right' key was pressed!\n";
     }

   }

   void SoundSpace::OnKeyUp(Events::KeyUp * event) {

     trace << "SoundSpace::OnKeyReleased\n";

     if (event->Key == Keys::Up) {
       trace << "'Up' key was released!\n";
     }
     else if (event->Key == Keys::Down) {
       trace << "'Down' key was released!\n";
     }
     else if (event->Key == Keys::Left) {
       trace << "'Left' key was released!\n";
     }
     else if (event->Key == Keys::Right) {
       trace << "'Right' key was released!\n";
     }

   }

   void SoundSpace::OnMouseDown(Events::MouseDown * event) {
     trace << "Mouse down!\n";

   }

   void SoundSpace::Serialize(Json::Value & root) {
   }

   void SoundSpace::Deserialize(Json::Value & root) {
   }



}