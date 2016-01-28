#include "AudioFMOD_Utilities.h"

namespace DCEngine {
  namespace Systems {
    
    EventInstanceInfo::EventInstanceInfo(unsigned id, FMOD::Studio::EventInstance * handle)
      : ID(id), Handle(handle)
    {
    }


  }
}