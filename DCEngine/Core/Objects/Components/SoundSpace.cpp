#include "SoundSpace.h"

#include "..\Entities\Space.h" // Need access to Space
#include "..\..\Systems\Audio\Audio.h" // PlaySound

#include "../../Engine/Engine.h" // TEMPORARY

namespace DCEngine {

  extern std::unique_ptr<Engine> Daisy; // TEMPORARY

   void SoundSpace::Initialize() {
     //Connect(space_, Events::LogicUpdate, SoundSpace::OnLogicUpdate); 
     TestMusic();

     // Testing Keyboard
     Connect(Daisy->GetKeyboard(), Events::KeyDown, SoundSpace::OnKeyDown);
     Connect(Daisy->GetKeyboard(), Events::KeyUp, SoundSpace::OnKeyUp);
     Connect(Daisy->GetMouse(), Events::MouseDown, SoundSpace::OnMouseDown);
  }

   void SoundSpace::TestMusic() {
     // THIS IS MY JAM
     using namespace Systems;
     std::string myJam = "spacejam.mp3";
     Daisy->GetSystem<Systems::Audio>(EnumeratedSystem::Audio)->PlayMusic(myJam);
   }

   void SoundSpace::OnLogicUpdate(Events::LogicUpdate* eventPtr) {
     trace << "SoundSpace::OnLogicUpdate - Dt: " << eventPtr->Dt << "\n";
   }

   void SoundSpace::OnKeyDown(Event * eventPtr) {
     auto event = (Events::KeyDown*)eventPtr;

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

   void SoundSpace::OnKeyUp(Event * eventPtr) {
     auto event = (Events::KeyUp*)eventPtr;

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

   void SoundSpace::OnMouseDown(Event * eventPtr) {
     auto event = (Events::MouseDown*)eventPtr;
     trace << "Mouse down!\n";

   }



}