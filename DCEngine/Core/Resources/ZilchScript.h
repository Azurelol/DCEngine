/*****************************************************************************/
/*!
@file       ZilchScript.h
@author     Christian Sagel
@par        email: c.sagel\@digipen.edu
@date       12/15/2015
@brief      A ZilchScript is.. 
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once
#include "ResourceReference.h"

namespace DCEngine {

  class ZilchScript : public Resource {
  public:

    ZilchDeclareDerivedType(ZilchScript, Resource);
    ZilchScript(std::string scriptFile);
    ~ZilchScript();
    static std::string Extension() { return ".Zilch"; }
    void Save(std::string& serializedData);
    bool Load();
    bool BuildDefaultFromTemplate();
    bool GenerateCode();

  private:

  };

  using ZilchScriptPtr = std::shared_ptr<ZilchScript>;

}

