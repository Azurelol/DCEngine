/*****************************************************************************/
/*!
@file      Bank.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/22/2016
@brief     A bank..
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Bank.h"

#include "../../Core/Engine/Engine.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  ZilchDefineType(Bank, "Bank", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    DCE_BINDING_DEFINE_PROPERTY(Bank, AssetPath);
    ZilchBindMethod(builder, type, &Bank::getBus, ZilchNoOverload, "Bus", "name");
    ZilchBindMethod(builder, type, &Bank::getVCA, ZilchNoOverload, "VCA", "name");
    ZilchBindMethod(builder, type, &Bank::Find, ZilchNoOverload, "Find", "name");
  }

  /**************************************************************************/
  /*!
  @brief  Bank constructor.
  */
  /**************************************************************************/
  Bank::Bank(std::string bankFile) : Resource("Bank", FileSystem::FileNoExtension(bankFile),
    bankFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief  Generates a list of SoundCues for every Event in the bank.
  */
  /**************************************************************************/
  void Bank::Add()
  {
    DCTrace << Name() << "::Bank::Add \n";
    // Add the bank to the audio system
    Daisy->getSystem<Systems::Audio>()->Add(AssetPath, Data);
  }

  /**************************************************************************/
  /*!
  @brief Returns the specified Bank resource.
  @param name The name of the bank.
  @return A reference to the Bank object.
  */
  /**************************************************************************/
  Bank* Bank::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getBank(name).get();
  }

  BankMap & Bank::All()
  {
    return *Daisy->getSystem<Systems::Content>()->AllBanks();
  }

  /**************************************************************************/
  /*!
  @brief  Generates the interface objects for the bank.
  */
  /**************************************************************************/
  void Bank::GenerateInterface()
  {
    // Create objects to represent VCAs/Buses
    for (auto& vca : Data.VCAs) {
      VCAs.insert(std::pair<Systems::VCAHandle, Systems::VCAPtr>(vca.first, vca.second));
    }
    for (auto& bus : Data.Buses) {
      Buses.insert(std::pair<Systems::BusHandle, Systems::BusPtr>(bus.first, bus.second));
    }
    DCTrace << Name() << "::Bank::Add: Finished! \n";
  }

  Bus * Bank::getBus(std::string name)
  {
    if (!Buses.count(name))
      return nullptr;

    return &Buses.at(name);
  }

  VCA * Bank::getVCA(std::string name)
  {
    if (!VCAs.count(name))
      return nullptr;

    return &VCAs.at(name);
  }

  /**************************************************************************/
  /*!
  @brief Retrieve a reference to the specified VCA.
  @param name The name of the VCA.
  @return A reference to the VCA.
  */
  /**************************************************************************/
  Systems::VCAPtr Bank::VCA(Systems::VCAHandle name)
  {
    if (!Data.VCAs.count(name)) {
      DCTrace << Name() << "::Bank::VCA: Could not find '" << name << "'!";
      return nullptr;
    }
    return Data.VCAs.at(name);
  }

  /**************************************************************************/
  /*!
  @brief Retrieves a reference to the container of all VCAs on the bank.
  @return A reference to the container.
  */
  /**************************************************************************/
  Systems::VCAContainer & Bank::AllVCAs()
  {
    return Data.VCAs;
  }

  /**************************************************************************/
  /*!
  @brief Retrieves a reference to the container of all Buses on the bank.
  @return A reference to the container.
  */
  /**************************************************************************/
  Systems::BusContainer & Bank::AllBuses()
  {
    return Data.Buses;
  }


  /**************************************************************************/
  /*!
  @brief Retrieve a reference to the specified Bus.
  @param name The name of the Bus.
  @return A reference to the Bus.
  */
  /**************************************************************************/
  Systems::BusPtr Bank::Bus(Systems::BusHandle name)
  {
    if (!Data.Buses.count(name)) {
      DCTrace << Name() << "::Bank::Bus: Could not find '" << name << "'!";
      return nullptr;
    }
    return Data.Buses.at(name);
  }

  //------------------------------------------------------------------------//

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  ZilchDefineType(VCA, "VCA", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    DCE_BINDING_DEFINE_PROPERTY(VCA, FaderLevel);
    ZilchBindMethod(builder, type, &VCA::Find, ZilchNoOverload, "Find", "name");
  }

  VCA::VCA(Systems::VCAPtr ptr) : Handle(ptr)
  {
  }

  VCA * VCA::Find(std::string name)
  {    
    for (auto& bank : Bank::All()) {
      if (auto vca = bank.second->getVCA(name)) {
        return vca;
      }
    }
    return nullptr;
  }

  void VCA::setFaderLevel(const float & level)
  {
    Handle->setFaderLevel(level);
  }

  const float & VCA::getFaderLevel()
  {
    float level; Handle->getFaderLevel(&level);
    return level;
  }

  //------------------------------------------------------------------------//

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch for reflection.
  */
  /**************************************************************************/
  ZilchDefineType(Bus, "Bus", DCEngineCore, builder, type) {
    DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    DCE_BINDING_DEFINE_PROPERTY(Bus, FaderLevel);
    DCE_BINDING_DEFINE_PROPERTY(Bus, Mute);
    DCE_BINDING_DEFINE_PROPERTY(Bus, Paused);
    ZilchBindMethod(builder, type, &Bus::Find, ZilchNoOverload, "Find", "name");
  }

  Bus::Bus(Systems::BusPtr ptr) : Handle(ptr)
  {
  }

  Bus * Bus::Find(std::string name)
  {
    for (auto& bank : Bank::All()) {
      if (auto bus = bank.second->getBus(name)) {
        return bus;
      }
    }
    return nullptr;
  }

  void Bus::setFaderLevel(const float & level)
  {
    Handle->setFaderLevel(level);
  }

  const float & Bus::getFaderLevel()
  {
    float level; Handle->getFaderLevel(&level);
    return level;
  }

  void Bus::setMute(const bool & mute)
  {
    Handle->setMute(mute);
  }

  const bool & Bus::getMute()
  {
    bool muted; Handle->getMute(&muted);
    return muted;
  }

  void Bus::setPaused(const bool & pause)
  {
    Handle->setPaused(pause);
  }

  const bool & Bus::getPaused()
  {
    bool paused; Handle->getPaused(&paused);
    return paused;
  }

}