#include "AudioFMOD.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the AudioFMOD class.
    */
    /**************************************************************************/
    AudioFMOD::AudioFMOD() {
    }
    AudioFMOD::~AudioFMOD() {
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD, configuring it.
    */
    /**************************************************************************/
    void AudioFMOD::Initialize() {
      
      // (?) Is there a better way to do this operation?
      FMOD::System* systemPtr = _system.get();
      if (FMOD::System_Create(&systemPtr) != FMOD_OK) {
        return;
      }

      int driverCount = 0;
      _system->getNumDrivers(&driverCount);

      if (driverCount == 0)
        return;

      // Initialize the instnace with 36 channels
      _system->init(36, FMOD_INIT_NORMAL, NULL);
    }

    /**************************************************************************/
    /*!
    \brief  Updates.
    */
    /**************************************************************************/
    void AudioFMOD::Update(float dt) {
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void AudioFMOD::Terminate() {
    }
  }
}