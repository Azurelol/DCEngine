/*****************************************************************************/
/*!
@file      Bank.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     A bank..
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

#include <FMOD\fmod_studio.hpp>
#include "../Systems/Audio/AudioFMOD_Utilities.h"

namespace DCEngine {
  
  class Bank;
  using BankHandle = std::string;
  using BankPtr = std::shared_ptr<Bank>;
  using BankMap = std::map<std::string, BankPtr>;

  namespace Systems {
    class AudioFMOD;
  }

  /**************************************************************************/
  /*!
  @brief VCA Interface for VCAs to Zilch.
  */
  /**************************************************************************/
  class VCA : public Object {
  public:
    ZilchDeclareDerivedType(VCA, Object);
    VCA(Systems::VCAPtr ptr);
    static VCA * Find(std::string name);
    void setFaderLevel(const float&);
    const float& getFaderLevel();
    Systems::VCAPtr Handle;
  };

  /**************************************************************************/
  /*!
  @brief Bus Interface for Buses to Zilch.
  */
  /**************************************************************************/
  class Bus : public Object {
  public:
    ZilchDeclareDerivedType(Bus, Object);
    Bus(Systems::BusPtr ptr);
    static Bus* Find(std::string name);
    void setFaderLevel(const float&);
    const float& getFaderLevel();
    void setMute(const bool&);
    const bool& getMute();
    void setPaused(const bool&);
    const bool& getPaused();
    Systems::BusPtr Handle;
  };

  /**************************************************************************/
  /*!
  @brief Bank A collection of sounds encapsulated..
  */
  /**************************************************************************/
  class Bank : public Resource {
    friend class Systems::AudioFMOD;
  public:
      
    // Properties
    DCE_DEFINE_PROPERTY(std::string, AssetPath);    
    // Zilch Getters
    Bus* getBus(std::string);
    VCA* getVCA(std::string);
    // Getters
    Systems::VCAPtr VCA(Systems::VCAHandle name);
    Systems::VCAContainer& AllVCAs();
    Systems::BusPtr Bus(Systems::BusHandle name);
    Systems::BusContainer& AllBuses();


    ZilchDeclareDerivedType(Bank, Resource);
    Bank(std::string name);
    ~Bank() {}
    void Add();
    static std::string Extension() { return ".Bank"; }
    static Bank* Find(std::string);
    static BankMap& All();

  private:
    void GenerateInterface();
    std::map<Systems::VCAHandle, DCEngine::VCA> VCAs;
    std::map<Systems::BusHandle, DCEngine::Bus> Buses;
    std::string AssetPath;
    Systems::BankInfo Data;

  };

}