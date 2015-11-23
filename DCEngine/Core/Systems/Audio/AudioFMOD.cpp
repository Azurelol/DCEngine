/******************************************************************************/
/*!
@file   AudioFMOD.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/16/2015
@brief  The interface for FMOD.
@note   References:
https://katyscode.wordpress.com/2012/10/05/cutting-your-teeth-on-fmod-
part-1-build-environment-initialization-and-playing-sounds/
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "AudioFMOD.h"

// Headers
#include "..\..\Components\EngineReference.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    \brief  Constructor for the AudioFMOD class.
    */
    /**************************************************************************/
    AudioFMOD::AudioFMOD() {
      MaxChannels = 36;
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

      // Check if the version of the DLL is the same as the libraries compiled
      unsigned int version;
      operationResult = system_.ptr->getVersion(&version);
      ErrorCheck(operationResult);
      if (version < FMOD_VERSION)
        DCTrace << "AudioFMOD::Initialize - Error: Outdated version!\n";

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
      operationResult = system_.ptr->init(MaxChannels, FMOD_INIT_NORMAL, NULL);
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
      //delete(MusicPtr);
      //delete CurrentChannel;
    }

    void AudioFMOD::ErrorCheck(FMOD_RESULT result) {
      if (result != FMOD_OK) {
        DCTrace << "AudioFMOD::ErrorCheck - FMOD error! (" << result << ")"
          << FMOD_ErrorString(result) << "\n";
      }
    }

    /**************************************************************************/
    /*!
    \brief  Loads a sound into memory in its entirety, decompressing it if 
            necessary.
    */
    /**************************************************************************/
    void AudioFMOD::CreateSound(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateSound: " <<  soundFile <<  "\n";
      OperationResult = system_.ptr->createSound(soundFile.c_str(), FMOD_CREATESAMPLE, 0, soundPtr);
      ErrorCheck(OperationResult);
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
            bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    void AudioFMOD::CreateStream(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateStream \n";
      OperationResult = system_.ptr->createStream(soundFile.c_str(), FMOD_DEFAULT, 0, soundPtr);
      ErrorCheck(OperationResult);
    }

    /**************************************************************************/
    /*!
    @brief  Plays a sound through FMOD.
    @param  soundPtr A pointer to the Sound data.
    @param  channel  A pointer to the Channel handle.
    @param  loop Whether the sound should be played in a loop.
    */
    /**************************************************************************/
    void AudioFMOD::PlaySound(FMOD::Sound* soundPtr, FMOD::Channel** channel, bool loop) {
      DCTrace << "AudioFMOD::PlaySound \n";
      if (loop) {
        soundPtr->setMode(FMOD_LOOP_NORMAL);
        soundPtr->setLoopCount(-1);
      }        
      else {
        soundPtr->setMode(FMOD_LOOP_OFF);
      }

      OperationResult = system_.ptr->playSound(soundPtr, NULL, 0, channel);
      ErrorCheck(OperationResult);
    }

    /**************************************************************************/
    /*!
    \brief  Resumes the playing of a sound through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(FMOD::Channel * channel)
    {
      OperationResult = channel->setPaused(false);
      ErrorCheck(OperationResult);
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::PauseSound(FMOD::Channel* channel)
    {
      OperationResult = channel->setPaused(true);
      ErrorCheck(OperationResult);
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(FMOD::Channel* channel)
    {
      OperationResult = channel->stop();
      ErrorCheck(OperationResult);
    }
    /**************************************************************************/
    /*!
    \brief  Releases a sound from FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ReleaseSound(FMOD::Sound* soundPtr) {
      OperationResult = soundPtr->release();
      ErrorCheck(OperationResult);
    }


  }
}