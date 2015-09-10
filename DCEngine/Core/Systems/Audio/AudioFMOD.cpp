#include "AudioFMOD.h"

#include "..\..\Debug\Debug.h"

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
    \brief  Creates a sound file for the AudioFMOD class.
    */
    /**************************************************************************/
    void AudioFMOD::CreateSound(FMOD::Sound** soundPtr, std::string& filePtr) {
      trace << "AudioFMOD::CreateSound \n";
      system_->createSound(filePtr.c_str(), FMOD_CREATESAMPLE, 0, soundPtr);
    }

    /**************************************************************************/
    /*!
    \brief  Plays a sound through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::PlaySound(FMOD::Sound* soundPtr, bool loop) {
      trace << "AudioFMOD::PlaySound \n";
      if (loop) {
        soundPtr->setMode(FMOD_LOOP_NORMAL);
        soundPtr->setLoopCount(-1);
      }        
      else {
        soundPtr->setMode(FMOD_LOOP_OFF);
      }

      // This cannot be very efficient, can it be?
      FMOD::Channel* channel = channel_.get();
      system_->playSound(soundPtr, NULL, 0, &channel);
    }

    /**************************************************************************/
    /*!
    \brief  Releases a sound, stopping it from playing.
    */
    /**************************************************************************/
    void AudioFMOD::ReleaseSound(FMOD::Sound* soundPtr) {
      soundPtr->release();
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD, configuring it.
    */
    /**************************************************************************/
    void AudioFMOD::Initialize() {
      
      // (?) Is there a better way to do this operation?
      FMOD::System* systemPtr = system_.get();
      if (FMOD::System_Create(&systemPtr) != FMOD_OK) {
        return;
      }

      int driverCount = 0;
      system_->getNumDrivers(&driverCount);

      if (driverCount == 0)
        return;

      // Initialize the instnace with 36 channels
      system_->init(36, FMOD_INIT_NORMAL, NULL);
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