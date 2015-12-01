/*****************************************************************************/
/*!
@file   PauseManager.h
@author Christian Sagel
@par    email: connor.tilley\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {

  class Space;
  class PauseManager : public Component {

  public:
    // Properties    
    
    // Methods
    PauseManager(Entity& owner) : Component(std::string("PauseManager"), owner) {}
    void Initialize();    
    void OnKeyDownEvent(Events::KeyDown* event);
    void OnKeyUpEvent(Events::KeyDown* event);
    void OnLogicUpdateEvent(Events::LogicUpdate * event);
    void EnablePauseMenu();
    void DisablePauseMenu();

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(PauseManager, Component);
    #endif

  private:
    Keys PauseKey;
    Boolean Paused = false;
    Space* PauseSpace;

    GameObjectPtr ButtonResume;
    GameObjectPtr ButtonHelp;
    GameObjectPtr ButtonCredits;
    GameObjectPtr ButtonQuit;

  };

}