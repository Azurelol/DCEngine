/******************************************************************************/
/*!
@file   CoreBindingTypes.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/7/2015
@brief  Binds types used in the engine so that Zilch can convert to them and
        back.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "CoreBinding.h"
#include "../Engine/Types.h"

ZilchDeclareRedirectType(std::string, Zilch::String);
ZilchDeclareRedirectType(glm::vec2, Zilch::Real2);
ZilchDeclareRedirectType(glm::vec3, Zilch::Real3);
ZilchDeclareRedirectType(glm::vec4, Zilch::Real4);


namespace DCEngine {

  class StringArray : public std::vector<std::string> {
  public:
    ZilchDeclareBaseType(StringArray, Zilch::TypeCopyMode::ReferenceType);
    void Add(const std::string& val);
    void Remove(const std::string& val);
    void Clear();


  };

}

