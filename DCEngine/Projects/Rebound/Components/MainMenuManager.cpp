#include "MainMenuManager.h"
#include "../../CoreComponents.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(MainMenuManager, "MainMenuManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelNewGame);
    DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelHelp);
    DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, LevelCredits);
    DCE_BINDING_DEFINE_PROPERTY(MainMenuManager, TransitionTime);
  }
  #endif  


  void MainMenuManager::Initialize()
  {
    // Set the references to the buttons
    ButtonNewGame = SpaceRef->FindObjectByName("NewGameButton");
    ButtonHelp = SpaceRef->FindObjectByName("HelpButton");
    ButtonCredits = SpaceRef->FindObjectByName("CreditsButton");
    ButtonExit = SpaceRef->FindObjectByName("ExitButton");
    
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

  }

  void MainMenuManager::OnKeyDownEvent(Events::KeyDown * event)
  {
  }

  void MainMenuManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

  void MainMenuManager::OnNewGameClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "MainMenuManager::OnNewGameClicked \n";
    // Load the first level
    SpaceRef->LoadLevel(LevelNewGame);
  }

  void MainMenuManager::OnHelpClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "MainMenuManager::OnHelpClicked \n";
  }

  void MainMenuManager::OnCreditsClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "MainMenuManager::OnCreditsClicked \n";
  }

  void MainMenuManager::OnExitClicked(Events::MouseClickedOn * event)
  {
    DCTrace << "MainMenuManager::OnExitClicked \n";
    Systems::DispatchSystemEvents::EngineExit();
  }

}