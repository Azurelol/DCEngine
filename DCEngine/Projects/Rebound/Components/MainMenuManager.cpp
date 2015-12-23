#include "MainMenuManager.h"
#include "../../CoreComponents.h"

namespace DCEngine {
  namespace Components
  {
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(MainMenuManager, "MainMenuManager", Rebound, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(MainMenuManager);
      DCE_BINDING_DEFINE_ATTRIBUTE(Level);
      DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelNewGame); // ->Attributes.push_back(attributeLevel);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLevelNewGame, attributeLevel);
      DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelHelp); // ->Attributes.push_back(attributeLevel);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLevelHelp, attributeLevel);
      DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelCredits); // ->Attributes.push_back(attributeLevel);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLevelCredits, attributeLevel);
      DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelCrunk); // ->Attributes.push_back(attributeLevel);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyLevelCrunk, attributeLevel);
      DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, TransitionTime);
    }
#endif  


    void MainMenuManager::Initialize()
    {
      // Set the references to the buttons
      ButtonNewGame = SpaceRef->FindObjectByName("NewGameButton");
      ButtonHelp = SpaceRef->FindObjectByName("HelpButton");
      ButtonCrunk = SpaceRef->FindObjectByName("CrunkButton");
      ButtonCredits = SpaceRef->FindObjectByName("CreditsButton");
      ButtonExit = SpaceRef->FindObjectByName("ExitButton");
      ButtonFullScreen = SpaceRef->FindObjectByName("FullScreen");
      ButtonConfirm = SpaceRef->FindObjectByName("ConfirmButton");
      ButtonCancel = SpaceRef->FindObjectByName("CancelButton");

      // Connect to their events
      if (ButtonNewGame)
        Connect(ButtonNewGame, Events::MouseClickedOn, MainMenuManager::OnNewGameClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonNewGame! \n";

      if (ButtonHelp)
        Connect(ButtonHelp, Events::MouseClickedOn, MainMenuManager::OnHelpClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonHelp! \n";

      if (ButtonCredits)
        Connect(ButtonCredits, Events::MouseClickedOn, MainMenuManager::OnCreditsClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCredits! \n";

      if (ButtonExit)
        Connect(ButtonExit, Events::MouseClickedOn, MainMenuManager::OnExitClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonExit! \n";

      if (ButtonCrunk)
        Connect(ButtonCrunk, Events::MouseClickedOn, MainMenuManager::OnCrunkClickedEvent);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCrunk! \n";

      if (ButtonFullScreen)
        Connect(ButtonFullScreen, Events::MouseClickedOn, MainMenuManager::OnFullScreenSwitch);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonFullScreen! \n";

      if (ButtonConfirm)
        Connect(ButtonConfirm, Events::MouseClickedOn, MainMenuManager::OnConfirmQuitClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonConfirm! \n";

      if (ButtonCancel)
        Connect(ButtonCancel, Events::MouseClickedOn, MainMenuManager::OnCancelQuitClicked);
      else
        DCTrace << "MainMenuManager::Initialize - Failed to find ButtonCancel! \n";

      DisplayConfirmQuit(false);

    }

    void MainMenuManager::OnKeyDownEvent(Events::KeyDown * event)
    {
    }

    void MainMenuManager::OnKeyUpEvent(Events::KeyDown * event)
    {
    }


    /*
      MAIN MENU
    */


    void MainMenuManager::OnNewGameClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;

      DCTrace << "MainMenuManager::OnNewGameClicked \n";
      // Load the first level
      SpaceRef->LoadLevel(LevelNewGame);
    }

    void MainMenuManager::OnHelpClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      DCTrace << "MainMenuManager::OnHelpClicked \n";
    }

    void MainMenuManager::OnCreditsClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      DCTrace << "MainMenuManager::OnCreditsClicked \n";
    }

    void MainMenuManager::OnExitClicked(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      DCTrace << "MainMenuManager::OnExitClicked \n";
      DisplayConfirmQuit(true);
    }

    void MainMenuManager::OnFullScreenSwitch(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      DCTrace << "MainMenuManager::OnFullScreenSwitch \n";
      auto fsevent = new Events::FullscreenEnabledEvent();
      Daisy->Dispatch<Events::FullscreenEnabledEvent>(fsevent);
      delete fsevent;
    }

    void MainMenuManager::OnCrunkClickedEvent(Events::MouseClickedOn * event)
    {
      if (DisplayConfirmationActive)
        return;
      SpaceRef->LoadLevel(LevelCrunk);
    }


    /*
      CONFIRMATION MENU
    */


    void MainMenuManager::OnConfirmQuitClicked(Events::MouseClickedOn * event)
    {
      if (!DisplayConfirmationActive)
        return;

      DCTrace << "MainMenuManager::OnConfirmQuitClicked \n";
      Systems::DispatchSystemEvents::EngineExit();
    }

    void MainMenuManager::OnCancelQuitClicked(Events::MouseClickedOn * event)
    {
      if (!DisplayConfirmationActive)
        return;

      DCTrace << "MainMenuManager::OnConfirmQuitClicked \n";
      DisplayConfirmQuit(false);
    }


    // Display the quit confirmation menu
    void MainMenuManager::DisplayConfirmQuit(bool display)
    {
      if (display) {
        DCTrace << "MainMenuManager::DisplayConfirmQuit - true \n";
        DisplayConfirmationActive = true;
        // Hide the main menu
        if (ButtonNewGame)
          ButtonNewGame->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonCrunk)
          ButtonCrunk->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonHelp)
          ButtonHelp->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonCredits)
          ButtonCredits->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonExit)
          ButtonExit->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonFullScreen)
          ButtonFullScreen->getComponent<Components::Sprite>()->setVisible(false);
        // Display the confirmation menu
        if (ButtonConfirm)
          ButtonConfirm->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonCancel)
          ButtonCancel->getComponent<Components::Sprite>()->setVisible(true);

      }

      else {
        DCTrace << "MainMenuManager::DisplayConfirmQuit - false \n";
        DisplayConfirmationActive = false;
        // Display the main menu
        if (ButtonNewGame)
          ButtonNewGame->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonCrunk)
          ButtonCrunk->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonHelp)
          ButtonHelp->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonCredits)
          ButtonCredits->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonExit)
          ButtonExit->getComponent<Components::Sprite>()->setVisible(true);
        if (ButtonFullScreen)
          ButtonFullScreen->getComponent<Components::Sprite>()->setVisible(true);
        // Hide the confirmation menu
        if (ButtonConfirm)
          ButtonConfirm->getComponent<Components::Sprite>()->setVisible(false);
        if (ButtonCancel)
          ButtonCancel->getComponent<Components::Sprite>()->setVisible(false);

      }
    }
  }

}