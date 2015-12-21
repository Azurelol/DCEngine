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
#include <Windows.h>
#include "PauseManager.h"

// Engine
#include "../../../Core/Engine/Engine.h"

namespace DCEngine {
  namespace Components {

#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(PauseManager, "PauseManager", Rebound, builder, type) {
    }
#endif  

    void PauseManager::ReloadPauseMainMenu()
    {
      PauseSpace->LoadLevel(std::string("PauseMenu"));
      //EnablePauseMenu();
      //return;
      ConnectPauseMainMenu();

    }

    void PauseManager::ConnectPauseMainMenu()
    {
      // Set the references to the objects within its space
      Background = PauseSpace->FindObjectByName("Background");
      ButtonResume = PauseSpace->FindObjectByName("ButtonResume");
      ButtonHelp = PauseSpace->FindObjectByName("ButtonHelp");
      ButtonCredits = PauseSpace->FindObjectByName("ButtonCredits");
      ButtonQuit = PauseSpace->FindObjectByName("SpriteQuit");

      ButtonConfirm = PauseSpace->FindObjectByName("ButtonConfirm");
      ButtonCancel = PauseSpace->FindObjectByName("ButtonCancel");
      BackgroundConfirm = PauseSpace->FindObjectByName("ConfirmationSprite");

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

      // Confirmation
      if (ButtonConfirm)
        Connect(ButtonConfirm, Events::MouseClickedOn, PauseManager::OnConfirmQuitClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonConfirm! \n";

      if (ButtonCancel)
        Connect(ButtonCancel, Events::MouseClickedOn, PauseManager::OnCancelQuitClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCancel! \n";


      DisplayConfirmQuit(false);
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

    /*
      ENABLE PAUSE MENU
    */

    void PauseManager::EnablePauseMenu()
    {
      Paused = true;
      DCTrace << "PauseManager::EnablePauseMenu \n";
      SpaceRef->getComponent<Components::TimeSpace>()->Pause();
      DisplayConfirmQuit(false);

      auto gs = GameSessionRef;

      // Create the PauseMenu space
      PauseSpace = GameSessionRef->CreateSpace("PauseSpace");
      // Load the pause level onto it
      PauseSpace->LoadLevel(std::string("PauseMenu"));

      ConnectPauseMainMenu();

    }

    /*
     DISABLE PAUSE MENU
    */
    void PauseManager::DisablePauseMenu()
    {
      Paused = false;
      DCTrace << "PauseManager::DisablePauseMenu \n";
      SpaceRef->getComponent<Components::TimeSpace>()->Pause();

      // Delete the PauseMenu space
      PauseSpace->Destroy();
      // Clear the references
      Background = nullptr;
      ButtonResume = nullptr;
      ButtonHelp = nullptr;
      ButtonCredits = nullptr;
      ButtonQuit = nullptr;
      ButtonBackHTP = nullptr;
      ButtonBackCredits = nullptr;


      BackgroundConfirm = nullptr;
      ButtonConfirm = nullptr;
      ButtonCancel = nullptr;


    }


    void PauseManager::OnBackButtonHTP(Events::MouseClickedOn* event)
    {
      const int KEYEVENT_KEYUP = 0x02;
      keybd_event(VK_ESCAPE, 0, 0, 0);              // press the Esc key
      keybd_event(VK_ESCAPE, 0, KEYEVENT_KEYUP, 0);
    }

    void PauseManager::OnBackButtonCredits(Events::MouseClickedOn* event)
    {
      const int KEYEVENT_KEYUP = 0x02;
      keybd_event(VK_ESCAPE, 0, 0, 0);              // press the Esc key
      keybd_event(VK_ESCAPE, 0, KEYEVENT_KEYUP, 0);
    }



    /*
      PAUSE MENU
    */

    void PauseManager::OnResumeClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      DCTrace << "PauseManager::OnResumeClicked \n";
      DisablePauseMenu();
    }

    void PauseManager::OnHelpClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
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
      if (DisplayConfirmationActive)
        return;
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
      if (DisplayConfirmationActive)
        return;
      DCTrace << "MainMenuManager::OnExitClicked \n";
      DisplayConfirmQuit(true);
    }

    /*
      CONFIRMATION MENU
    */

    void PauseManager::OnConfirmQuitClicked(Events::MouseClickedOn * event)
    {
      if (!DisplayConfirmationActive)
        return;

      DCTrace << "MainMenuManager::OnConfirmQuitClicked \n";
      Systems::DispatchSystemEvents::EngineExit();
    }

    void PauseManager::OnCancelQuitClicked(Events::MouseClickedOn * event)
    {
      if (!DisplayConfirmationActive)
        return;

      DCTrace << "MainMenuManager::OnConfirmQuitClicked \n";
      DisplayConfirmQuit(false);
    }

    // Display the quit confirmation menu
    void PauseManager::DisplayConfirmQuit(bool display)
    {
      if (display) {
        DCTrace << "MainMenuManager::DisplayConfirmQuit - true \n";
        DisplayConfirmationActive = true;
        // Hide the main menu
        if (Background)
          Background->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonResume)
          ButtonResume->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonHelp)
          ButtonHelp->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonCredits)
          ButtonCredits->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonQuit)
          ButtonQuit->getComponent<Components::Sprite>()->setVisible(false);
        // Display the confirmation menu
        if (ButtonConfirm)
          ButtonConfirm->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonCancel)
          ButtonCancel->getComponent<Components::Sprite>()->setVisible(true);
        if (BackgroundConfirm)
          BackgroundConfirm->getComponent<Components::Sprite>()->setVisible(true);

      }

      else {
        DCTrace << "MainMenuManager::DisplayConfirmQuit - false \n";
        DisplayConfirmationActive = false;
        // Display the main menu
        if (Background)
          Background->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonResume)
          ButtonResume->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonHelp)
          ButtonHelp->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonCredits)
          ButtonCredits->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonQuit)
          ButtonQuit->getComponent<Components::Sprite>()->setVisible(true);
        // Hide the confirmation menu
        if (ButtonConfirm)
          ButtonConfirm->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonCancel)
          ButtonCancel->getComponent<Components::Sprite>()->setVisible(false);
        if (BackgroundConfirm)
          BackgroundConfirm->getComponent<Components::Sprite>()->setVisible(false);

      }
    }

  }
}