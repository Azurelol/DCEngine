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

  void PauseManager::ReloadPauseMainMenu()
  {
	  PauseSpace->LoadLevel(std::string("PauseMenu"));
	  ButtonResume = PauseSpace->FindObjectByName("ButtonResume");
	  ButtonHelp = PauseSpace->FindObjectByName("ButtonHelp");
	  ButtonCredits = PauseSpace->FindObjectByName("ButtonCredits");
	  ButtonQuit = PauseSpace->FindObjectByName("SpriteQuit");
	  // Connect to its events
	  if (ButtonResume)
		  Connect(ButtonResume, Events::MouseClickedOn, PauseManager::OnResumeClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonNewGame! \n";

	  if (ButtonHelp)
		  Connect(ButtonHelp, Events::MouseClickedOn, PauseManager::OnHelpClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonHelp! \n";

	  if (ButtonCredits)
		  Connect(ButtonCredits, Events::MouseClickedOn, PauseManager::OnCreditsClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonCredits! \n";

	  if (ButtonQuit)
		  Connect(ButtonQuit, Events::MouseClickedOn, PauseManager::OnQuitClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonExit! \n";
  }

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
	  {
		  if (IsInCredits)
		  {
			  IsInCredits = false;
			  ReloadPauseMainMenu();
			  ButtonBackCredits = NULL;
		  }
		  else if (IsInHowToPlay)
		  {
			  IsInHowToPlay = false;
			  ReloadPauseMainMenu();
			  ButtonBackHTP = NULL;
		  }
		  else
		  {
			  DisablePauseMenu();
		  }
	  }
    }
    
  }

  void PauseManager::OnKeyUpEvent(Events::KeyDown * event)
  {

  }

  /*PauseManager::~PauseManager()
  {
	  if (PauseSpace)
	  {
		  PauseSpace->Destroy();
	  }
  }*/

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
    // Set the references to the objects within its space
    ButtonResume = PauseSpace->FindObjectByName("ButtonResume");
    ButtonHelp = PauseSpace->FindObjectByName("ButtonHelp");
    ButtonCredits = PauseSpace->FindObjectByName("ButtonCredits");
    ButtonQuit = PauseSpace->FindObjectByName("SpriteQuit");
    // Connect to its events
    if (ButtonResume)
      Connect(ButtonResume, Events::MouseClickedOn, PauseManager::OnResumeClicked);
    else
      DCTrace << "PauseManager::Initialize - Failed to find ButtonNewGame! \n";

    if (ButtonHelp)
      Connect(ButtonHelp, Events::MouseClickedOn, PauseManager::OnHelpClicked);
    else
      DCTrace << "PauseManager::Initialize - Failed to find ButtonHelp! \n";

    if (ButtonCredits)
      Connect(ButtonCredits, Events::MouseClickedOn, PauseManager::OnCreditsClicked);
    else
      DCTrace << "PauseManager::Initialize - Failed to find ButtonCredits! \n";

    if (ButtonQuit)
      Connect(ButtonQuit, Events::MouseClickedOn, PauseManager::OnQuitClicked);
    else
      DCTrace << "PauseManager::Initialize - Failed to find ButtonExit! \n";

  }

  void PauseManager::OnBackButtonHTP(Events::MouseClickedOn* event)
  {
	  IsInHowToPlay = false;
	  PauseSpace->LoadLevel(std::string("PauseMenu"));
	  ButtonResume = PauseSpace->FindObjectByName("ButtonResume");
	  ButtonHelp = PauseSpace->FindObjectByName("ButtonHelp");
	  ButtonCredits = PauseSpace->FindObjectByName("ButtonCredits");
	  ButtonQuit = PauseSpace->FindObjectByName("SpriteQuit");
	  // Connect to its events
	  if (ButtonQuit)
		  Connect(ButtonQuit, Events::MouseClickedOn, PauseManager::OnQuitClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonExit! \n";
	  if (ButtonResume)
		  Connect(ButtonResume, Events::MouseClickedOn, PauseManager::OnResumeClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonNewGame! \n";

	  if (ButtonHelp)
		  Connect(ButtonHelp, Events::MouseClickedOn, PauseManager::OnHelpClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonHelp! \n";

	  if (ButtonCredits)
		  Connect(ButtonCredits, Events::MouseClickedOn, PauseManager::OnCreditsClicked);
	  else
		  DCTrace << "PauseManager::Initialize - Failed to find ButtonCredits! \n";
	  ButtonBackHTP = NULL;
  }

  void PauseManager::OnBackButtonCredits(Events::MouseClickedOn* event)
  {
	  IsInCredits = false;
	  ReloadPauseMainMenu();
	  ButtonBackCredits = NULL;
  }

  void PauseManager::DisablePauseMenu()
  {
    Paused = false;
    DCTrace << "PauseManager::DisablePauseMenu \n";
    SpaceRef->getComponent<TimeSpace>()->Pause();

    // Delete the PauseMenu space
    PauseSpace->Destroy();
    // Clear the references
    ButtonResume = nullptr;
    ButtonHelp = nullptr;
    ButtonCredits = nullptr;
    ButtonQuit = nullptr;
    
  }

  void PauseManager::OnResumeClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "PauseManager::OnResumeClicked \n";
    DisablePauseMenu();
  }

  void PauseManager::OnHelpClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "PauseManager::OnHelpClicked \n";
	std::string tmp("HowToPlay");
	IsInCredits = true;
	PauseSpace->LoadLevel(tmp);
	ButtonBackHTP = PauseSpace->FindObjectByName("BackButtonHTP");
	if (ButtonBackHTP)
		Connect(ButtonBackHTP, Events::MouseClickedOn, PauseManager::OnBackButtonHTP);
	else
		DCTrace << "PauseManager::Initialize - Failed to find ButtonBackHTP! \n";

  }

  void PauseManager::OnCreditsClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "PauseManager::OnCreditsClicked \n";
	std::string tmp("CreditsLevel");
	IsInHowToPlay = true;
	PauseSpace->LoadLevel(tmp);
	ButtonBackCredits = PauseSpace->FindObjectByName("BackButtonCredits");
	if (ButtonBackCredits)
		Connect(ButtonBackCredits, Events::MouseClickedOn, PauseManager::OnBackButtonCredits);
	else
		DCTrace << "PauseManager::Initialize - Failed to find OnBackButtonCredits! \n";
  }

  void PauseManager::OnQuitClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "PauseManager::OnQuitClicked \n";
    Systems::DispatchSystemEvents::EngineExit();
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