#include "ReflectionZilch.h"

namespace DCEngine {
  namespace Systems {
    
    ReflectionZilch::ReflectionZilch()
    {
      // Any one time startup and static initialization that Zilch needs to do
      // This also registers a custom asswe
      //Zilch::ZilchSetup setup(Zilch::StartupFlags::None);
    }

    void ReflectionZilch::Initialize()
    {
    }

    void ReflectionZilch::Update(float dt)
    {
    }

    void ReflectionZilch::Terminate()
    {
    }

  }
}