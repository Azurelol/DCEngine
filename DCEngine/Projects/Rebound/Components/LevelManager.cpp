#include "LevelManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(LevelManager, "LevelManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, NextLevel);
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, LoadingTime);
	DCE_BINDING_DEFINE_PROPERTY(LevelManager, Timer);
	DCE_BINDING_DEFINE_PROPERTY(LevelManager, TimerStarted);
  }
  #endif

  void LevelManager::Initialize()
  {
  }

  void LevelManager::OnKeyDownEvent(Events::KeyDown * event)
  {

  }

  void LevelManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

  void LevelManager::OnCollisionStartedEvent(Events::CollisionStarted * event)
  {
	  if (event->OtherObject->getComponent<PlayerController>())
	  {
		  TimerStarted = true;
		 // this->SpaceRef->CreateObject()
	  }
  }

  void LevelManager::OnCollisionEndedEvent(Events::CollisionEnded * event)
  {
  }

  void LevelManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
	  if (!TimerStarted)
		  return;

	  Timer += event->Dt;
	  if (Timer > LoadingTime)
	  {
		  this->SpaceRef->LoadLevel(NextLevel);
	  }
  }

}