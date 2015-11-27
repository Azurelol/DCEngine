#include "MusicManager.h"
#include "ReboundEngineReference.h"

namespace DCEngine {

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(MusicManager, "MusicManager", Rebound, builder, type) {
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, CurrentTrack);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, LastTrack);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeInTime);
    DCE_BINDING_DEFINE_PROPERTY(MusicManager, FadeOutTime);
  }
  #endif

  void MusicManager::Initialize()
  {
  }

  void MusicManager::OnKeyDownEvent(Events::KeyDown * event)
  {
  }

  void MusicManager::OnKeyUpEvent(Events::KeyDown * event)
  {
  }

}