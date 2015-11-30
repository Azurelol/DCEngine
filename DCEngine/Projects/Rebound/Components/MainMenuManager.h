/*****************************************************************************/
/*!
@file   MainMenuManager.h
@author Christian Sagel
@par    email: connor.tilley\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
    
  class MainMenuManager : public Component {

  public:
    // Properties    
    DCE_DEFINE_PROPERTY(LevelHandle, LevelNewGame);
    DCE_DEFINE_PROPERTY(LevelHandle, LevelHelp);
    DCE_DEFINE_PROPERTY(LevelHandle, LevelCredits);
    DCE_DEFINE_PROPERTY(Real, TransitionTime);

    // Methods
    MainMenuManager(Entity& owner) : Component(std::string("MainMenuManager"), owner) {}
    void Initialize();
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyDown* event);

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(MainMenuManager, Component);
    #endif

  private:

    LevelHandle LevelNewGame;
    LevelHandle LevelHelp;
    LevelHandle LevelCredits;
    Real TransitionTime = 0;

    GameObjectPtr ButtonNewGame;
    GameObjectPtr ButtonHelp;
    GameObjectPtr ButtonCredits;
    GameObjectPtr ButtonExit;

  };

}