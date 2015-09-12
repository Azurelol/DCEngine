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
      Terminate();      
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD, configuring it.
    */
    /**************************************************************************/
    void AudioFMOD::Initialize() {
      FMOD_RESULT operationResult;

      //operationResult = FMOD::System_Create(&_system.ptr);  
      // Check FMOD interface object
      //operationResult = FMOD::System_Create(&system_);
      //ErrorCheck(operationResult);
      // Check if the version of the DLL is the same as the libraries compiled
      unsigned int version;
      operationResult = system_.ptr->getVersion(&version);
      ErrorCheck(operationResult);
      if (version < FMOD_VERSION)
        trace << "AudioFMOD::Initialize - Error: Outdated version!\n";
      // Check the number of sound cards
      int driverCount = 0;
      system_.ptr->getNumDrivers(&driverCount);
      // If there are no sound cards, disable output
      if (driverCount == 0) {
        operationResult = system_.ptr->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ErrorCheck(operationResult);
      }
      // If there is at least one sound card
      else {
        //FMOD_SPEAKERMODE speakermode;
        //system_->getDriverInfo(0, &speakermode);
        //result = system_->getDriverInfo(0, &)
      }

      // Initialize the instnace with 36 channels
      operationResult = system_.ptr->init(36, FMOD_INIT_NORMAL, NULL);
      ErrorCheck(operationResult);
    }

    /**************************************************************************/
    /*!
    \brief  Updates.
    */
    /**************************************************************************/
    void AudioFMOD::Update(float dt) {
      system_.ptr->update();
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void AudioFMOD::Terminate() {
      //delete MusicPtr;
      //delete CurrentChannel;
    }

    void AudioFMOD::ErrorCheck(FMOD_RESULT result) {
      if (result != FMOD_OK) {
        trace << "AudioFMOD::ErrorCheck - FMOD error! (" << result << ")"
          << FMOD_ErrorString(result) << "\n";
      }
    }

    /**************************************************************************/
    /*!
    \brief  Loads a sound into memory in its entirety, decompressing it if 
            necessary.
    */
    /**************************************************************************/
    void AudioFMOD::CreateSound(FMOD::Sound** soundPtr, std::string& filePtr) {
      trace << "AudioFMOD::CreateSound \n";
      FMOD_RESULT result;
      result = system_.ptr->createSound(filePtr.c_str(), FMOD_CREATESAMPLE, 0, soundPtr);
      ErrorCheck(result);
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
            bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    void AudioFMOD::CreateStream(FMOD::Sound ** streamPtr, std::string & filePtr) {
      trace << "AudioFMOD::CreateStream \n";
      FMOD_RESULT result;
      result = system_.ptr->createStream(filePtr.c_str(), FMOD_DEFAULT, 0, streamPtr);
      ErrorCheck(result);
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

      FMOD_RESULT result;
      result = system_.ptr->playSound(soundPtr, NULL, 0, &CurrentChannel);
      ErrorCheck(result);
    }

    /**************************************************************************/
    /*!
    \brief  Plays a music file through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::PlayMusic(std::string & filePath) {
      std::string resourceLocation("Projects/Resources/Sounds/");
      CreateStream(&MusicPtr, resourceLocation + filePath);
      PlaySound(MusicPtr, true);
    }

    /**************************************************************************/
    /*!
    \brief  Stops the currently playing music file through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::StopMusic() {
      ReleaseSound(MusicPtr);
    }

    /**************************************************************************/
    /*!
    \brief  Releases a sound, stopping it from playing.
    */
    /**************************************************************************/
    void AudioFMOD::ReleaseSound(FMOD::Sound* soundPtr) {
      FMOD_RESULT result;
      result = soundPtr->release();
      ErrorCheck(result);
    }


  }
}