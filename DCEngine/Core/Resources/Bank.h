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
  class Bank : public Resource {
  public:
      
    // Properties
    DCE_DEFINE_PROPERTY(std::string, AssetPath);

    



    ZilchDeclareDerivedType(Bank, Resource);
    Bank(std::string name);
    ~Bank() {}
    void Add();
    static std::string Extension() { return ".Bank"; }
    static BankPtr Find(std::string);

    struct BankData {
      FMOD::Studio::Bank* Handle;
      FMOD::Studio::Bank* operator->() { return Handle; }
    };

  private:
    std::string AssetPath;
    BankData Data;

  };

}