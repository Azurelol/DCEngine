#include "LevelManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(LevelManager, "LevelManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, NextLevel);
    DCE_BINDING_DEFINE_PROPERTY(LevelManager, LoadingTime);
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
  }

  void LevelManager::OnCollisionEndedEvent(Events::CollisionEnded * event)
  {
  }

  void LevelManager::OnLogicUpdateEvent(Events::LogicUpdate * event)
  {
  }

}