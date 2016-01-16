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

      // Initialize the low level API
      InitializeLowLevelAPI();
      // Initialize the Studio API
      InitializeStudioAPI();
    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD's Low Level API.
    */
    /**************************************************************************/
    void AudioFMOD::InitializeLowLevelAPI()
    {
      FMOD_RESULT operationResult;
      // Check if the version of the DLL is the same as the libraries compiled
      unsigned int version;
      operationResult = System.ptr->getVersion(&version);
      if (version < FMOD_VERSION)
        DCTrace << "AudioFMOD::Initialize - Error: Outdated version!\n";

      // Check the number of sound cards
      int driverCount = 0;
      System.ptr->getNumDrivers(&driverCount);

      // If there are no sound cards, disable output
      if (driverCount == 0) {
        operationResult = System.ptr->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ErrorCheck(operationResult);
      }
      // If there is at least one sound card
      else {
        //FMOD_SPEAKERMODE speakermode;
        //System->getDriverInfo(0, &speakermode);
        //result = System->getDriverInfo(0, &)
      }

      // Initialize the instnace with 36 channels
      operationResult = System.ptr->init(MaxChannels, FMOD_INIT_NORMAL, NULL);
      ErrorCheck(operationResult);

    }

    /**************************************************************************/
    /*!
    \brief  Initializes FMOD's Studio API.
    */
    /**************************************************************************/
    void AudioFMOD::InitializeStudioAPI()
    {
      // Create the sound system
      ErrorCheck(System->create(&System.Handle));
      // Initialize it
      ErrorCheck(System->initialize(MaxChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));

    }


    /**************************************************************************/
    /*!
    \brief  Updates.
    */
    /**************************************************************************/
    void AudioFMOD::Update(float dt) {
      // Update the low level API system
      ErrorCheck(System.ptr->update());
      // Update the 'Studio' API system
      //ErrorCheck(System->update());
    }

    /**************************************************************************/
    /*!
    \brief  Terminates.
    */
    /**************************************************************************/
    void AudioFMOD::Terminate() {
      //System.ptr->
      //System.ptr->getChannelsPlaying().

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
      ErrorCheck(System.ptr->createSound(soundFile.c_str(), FMOD_CREATESAMPLE, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    \brief  Opens a file and buffers it a piece at a time, decompressing each
            bufferred segment on the fly during playback.
    */
    /**************************************************************************/
    void AudioFMOD::CreateStream(std::string& soundFile, FMOD::Sound** soundPtr) {
      DCTrace << "AudioFMOD::CreateStream \n";
      ErrorCheck(System.ptr->createStream(soundFile.c_str(), FMOD_DEFAULT, 0, soundPtr));
    }

    /**************************************************************************/
    /*!
    @brief  Adds a bank to the FMOD Studio system.
    @param  handle The handle to be used to access the bank.
    @param  path The filepath where the bank is located.
    @return A pointer to the bank.
    */
    /**************************************************************************/
    FMOD::Studio::Bank * AudioFMOD::LoadBankFromFile(std::string handle, std::string & path)
    {
      DCTrace << "AudioFMOD::LoadBankFromFile - Loading: '" << handle << "' from file: '"<< path << "'\n";
      FMOD::Studio::Bank* newBank;
      // Load the bank into the sound system
      ErrorCheck(System->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &newBank));
      // Add it to the container of active banks
      ActiveBanks.insert(std::pair<std::string, FMOD::Studio::Bank*>(handle, newBank));
      return newBank;
    }

    /**************************************************************************/
    /*!
    @brief  Retrieves a pointer to a given sound bank.
    @param  handle The name of the sound bank.
    @return A pointer to the bank.
    */
    /**************************************************************************/
    FMOD::Studio::Bank * AudioFMOD::getBank(std::string handle)
    {
      // If the bank could not be found
      if (!ActiveBanks.count(handle))
        return nullptr;
      // If the bank could be found...
      return ActiveBanks.at(handle);
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

      //FMOD::Channel::
      ErrorCheck(System.ptr->playSound(soundPtr, NULL, 0, channel));
    }

    /**************************************************************************/
    /*!
    \brief  Resumes the playing of a sound through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ResumeSound(FMOD::Channel * channel)
    {
      ErrorCheck(channel->setPaused(false));
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::PauseSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->setPaused(true));
    }

    /**************************************************************************/
    /*!
    \brief  Stops a sound from playing through FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::StopSound(FMOD::Channel* channel)
    {
      ErrorCheck(channel->stop());
    }
    /**************************************************************************/
    /*!
    \brief  Releases a sound from FMOD.
    */
    /**************************************************************************/
    void AudioFMOD::ReleaseSound(FMOD::Sound* soundPtr) {
      ErrorCheck(soundPtr->release());
    }


  }
}