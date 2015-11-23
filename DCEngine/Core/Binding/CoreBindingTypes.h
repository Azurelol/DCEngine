/******************************************************************************/
/*!
@file   CoreBindingTypes.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/7/2015
@brief  Binds types used in the engine so that Zilch can convert to them and
        back.
*/
/******************************************************************************/
#pragma once
#include "CoreBinding.h"
#include "../Engine/Types.h"

ZilchDeclareRedirectType(std::string, Zilch::String);
ZilchDeclareRedirectType(glm::vec2, Zilch::Real2);
ZilchDeclareRedirectType(glm::vec3, Zilch::Real3);
ZilchDeclareRedirectType(glm::vec4, Zilch::Real4);

