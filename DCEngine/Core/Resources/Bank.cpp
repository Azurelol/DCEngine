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
    DCE_BINDING_DEFINE_PROPERTY(Bank, AssetPath);
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


  /**************************************************************************/
  /*!
  @brief  CollisionGroup constructor.
  */
  /**************************************************************************/
  Bank::Bank(std::string bankFile) : Resource("Bank", FileSystem::FileNoExtension(bankFile),
                                              bankFile)
  {
  }

  /**************************************************************************/
  /*!
  @brief Returns the specified Bank resource.
  @param name The name of the bank.
  @return A reference to the Bank object.
  */
  /**************************************************************************/
  BankPtr Bank::Find(std::string name)
  {
    return Daisy->getSystem<Systems::Content>()->getBank(name);
  }

  /**************************************************************************/
  /*!
  @brief  Generates a list of SoundCues for every Event in the bank.
  */
  /**************************************************************************/
  void Bank::Add()
  {
    // Add the bank to the audio system
    Daisy->getSystem<Systems::Audio>()->Add(AssetPath, Data);
    DCTrace << Name() << "::Bank::Add \n";
  }

}