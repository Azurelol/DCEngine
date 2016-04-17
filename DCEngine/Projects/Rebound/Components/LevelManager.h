/*****************************************************************************/
/*!
@file   LevelManager.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/26/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

namespace DCEngine {
  namespace Components
  {
    class Fade;
    class LevelManager : public Component {

    public:
      // Properties
      DCE_DEFINE_PROPERTY(String, NextLevel);
      DCE_DEFINE_PROPERTY(Real, LoadingTime);
      DCE_DEFINE_PROPERTY(Real, Timer);
      DCE_DEFINE_PROPERTY(Boolean, TimerStarted);
	  DCE_DEFINE_PROPERTY(Boolean, Active);

      // Methods
      LevelManager(Entity& owner) : Component(std::string("LevelManager"), owner) {}
      void Initialize();
      void OnMouseClickedOnEvent(Events::MouseClickedOn* event);
      void OnKeyDownEvent(Events::KeyDown* event);
      void OnKeyUpEvent(Events::KeyDown* event);
      void OnCollisionStartedEvent(Events::CollisionStarted* event);
      void OnCollisionEndedEvent(Events::CollisionEnded* event);
      void OnLogicUpdateEvent(Events::LogicUpdate * event);


#if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(LevelManager, Component);
#endif

    private:
      String NextLevel;
      Real LoadingTime = 1;
      Real Timer = 1;
      Fade* FadeRef;
      Boolean TimerStarted = false;
	  Boolean Active = true;
	  Transform* TransformRef;

    };
  }

}