/*****************************************************************************/
/*!
@file   PauseManager.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   10/01/2015
@note   This file includes all the headers for the components that Rebound
uses.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PauseManager.h"

// Engine
#include "../../../Core/Engine/Engine.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(PauseManager, "PauseManager", Rebound, builder, type) {
  }
  #endif  

  void PauseManager::Initialize()
  {
    Connect(Daisy->getKeyboard(), Events::KeyDown, PauseManager::OnKeyDownEvent);
  }

  void PauseManager::OnKeyDownEvent(Events::KeyDown * event)
  {
    if (event->Key == Keys::Escape) {

      if (!Paused)
        EnablePauseMenu();
      else if (Paused)
        DisablePauseMenu();
    }
    
  }

  void PauseManager::OnKeyUpEvent(Events::KeyDown * event)
  {

  }

  void PauseManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
  }

  void PauseManager::EnablePauseMenu()
  {
    Paused = true;
    DCTrace << "PauseManager::EnablePauseMenu \n";
    SpaceRef->getComponent<TimeSpace>()->Pause();

    auto gs = GameSessionRef;

    // Create the PauseMenu space
    PauseSpace = GameSessionRef->CreateSpace("PauseSpace");
    // Load the pause level onto it
    PauseSpace->LoadLevel(std::string("PauseMenu"));

  }

  void PauseManager::DisablePauseMenu()
  {
    Paused = false;
    DCTrace << "PauseManager::DisablePauseMenu \n";
    SpaceRef->getComponent<TimeSpace>()->Pause();

    // Delete the PauseMenu space
    PauseSpace->Destroy();
    
  }



  /**************************************************************************/
  /*!
  @brief  Displays a Pause Menu.
  @todo   Implement the pause menu in a different way.
  */
  /**************************************************************************/
  //void PauseMenu()
  //{
  //  // Pause the engine (Physics, Input, Events)
  //  auto pause = new Events::EnginePause();
  //  Daisy->Dispatch<Events::EnginePause>(pause);
  //  delete pause;

  //  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiSetCond_FirstUseEver);

  //  
  //  ImGui::Begin("Pause Menu", &PauseMenuEnabled);
  //  if (ImGui::Button("Pause")) {

  //  }
  //  if (ImGui::Button("Resume Game")) {
  //    // Unpause the engine (Physics, Input, Events)
  //    auto resume = new Events::EngineResume();
  //    Daisy->Dispatch<Events::EngineResume>(resume);
  //    delete resume;
  //    PauseMenuEnabled = false;
  //  }

  //  if (ImGui::Button("How to Play")) {

  //  }


  //  if (ImGui::Button("Quit Game")) {
  //    // Destructive confirmation
  //    auto exitEvent = new Events::EngineExit();
  //    Daisy->Dispatch<Events::EngineExit>(exitEvent);
  //    delete exitEvent;
  //  }

  //  ImGui::End();

  //}




}