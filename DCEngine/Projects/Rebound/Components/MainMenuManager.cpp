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
  }

  void MainMenuManager::OnKeyDownEvent(Events::KeyDown * event)
  {
  }

  void MainMenuManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

}