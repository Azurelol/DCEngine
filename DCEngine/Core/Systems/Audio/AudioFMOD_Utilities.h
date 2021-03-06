/******************************************************************************/
/*!
@file   AudioFMOD_Utilities.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/9/2015
@brief  
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include <FMOD\fmod_studio.hpp>
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>

namespace DCEngine {
  namespace Systems {
   

    /*==================
          ALIASES
    ===================*/
    // Event Descriptions
    using EventDescriptionHandle = const std::string&;
    // Event Instances
    struct EventInstanceInfo {    
      EventInstanceInfo(unsigned id, FMOD::Studio::EventInstance* handle);
      unsigned ID; 
      FMOD::Studio::EventInstance* Handle;
    };
    using EventInstanceHandle = FMOD::Studio::EventInstance*;
    using EventInstanceMap = std::map<std::string, FMOD::Studio::EventInstance*>;
    // Event Parameters
    struct EventParameterInfo {
      std::string Name;
      float Minimum;
      float Maximum;
      FMOD_STUDIO_PARAMETER_TYPE Type;
    };
    using EventParameterHandle = std::string;
    using EventParameterInfoContainer = std::vector<EventParameterInfo>;
    using EventParameterContainer = std::vector<FMOD::Studio::ParameterInstance*>;
    // VCA
    using VCAHandle = std::string;
    using VCAPtr = FMOD::Studio::VCA*;
    using VCAContainer = std::map<VCAHandle, FMOD::Studio::VCA*>;
    // Bus
    using BusHandle = std::string;
    using BusPtr = FMOD::Studio::Bus*;
    using BusContainer = std::map<BusHandle, FMOD::Studio::Bus*>;
    // Banks
    struct BankInfo {
      std::string Name;
      FMOD::Studio::Bank* Handle;
      VCAContainer VCAs;
      BusContainer Buses;
      FMOD::Studio::Bank* operator->() { return Handle; }
    };
    using BanksContainer = std::map<std::string, BankInfo>;
    // Containers
    using EventDescriptionMap = std::map<std::string, FMOD::Studio::EventDescription*>;
    // Channels
    using ChannelMap = std::map<FMOD::Sound*, FMOD::Channel*>;
    using GroupMap = std::map<std::string, FMOD::ChannelGroup*>;

    /*==================
        DATA STRUCTS
    ===================*/
    // Settings 
    struct AudioFMODSettings {
      unsigned MaxChannels;
      unsigned Volume;
      unsigned Pitch;
      bool Paused;
      float Level;
      bool Muted;
    };

    // Smart pointer wrapper for raw FMOD pointers
    class FMODSystemPtr {
    public:
      FMOD::Studio::System* Studio;
      FMOD::System* LowLevel;

      FMODSystemPtr() : Studio(nullptr)
      {
        // Create the low level API system
        if (FMOD::System_Create(&LowLevel) != FMOD_OK) {
          LowLevel = nullptr;
          //throw something;
        }
        // Create the Studio API system
        //if (FMOD::Studio::System::create(&Handle) != FMOD_OK)
        //  Handle = nullptr;
      }
      FMODSystemPtr(FMODSystemPtr&&) = default;
      FMODSystemPtr& operator=(FMODSystemPtr&&) = default;
      FMODSystemPtr(const FMODSystemPtr&) = delete;
      FMODSystemPtr& operator=(const FMODSystemPtr&) = delete;
      FMOD::Studio::System* operator->() {
        return Studio;
      }
      ~FMODSystemPtr() {
        //auto result = LowLevel->release();
        Studio->release();
      }
    };

  }
}



