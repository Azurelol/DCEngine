/******************************************************************************/
/*!
@file   CoreBindingEvents.h
@author Gabriel Neumann
@par    email: g.neumann\@digipen.edu
@date   2/27/2016
@brief  Binds enums used in the engine so that Zilch can convert to them and
back.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Precompiled.h"
#include "CoreBindingObjects.h"
#include "CoreBinding.h"
#include "CoreBindingTypes.h"

#include"../Engine/Types.h"
#include "../Events/EventReference.h"
#include "../Events/InputEvents.h"

#include <ZILCH\Zilch.hpp>

ZilchDeclareExternalBaseType(DCEngine::Direction, Zilch::TypeCopyMode::ReferenceType);
ZilchDeclareExternalBaseType(DCEngine::Keys, Zilch::TypeCopyMode::ReferenceType);
ZilchDeclareExternalBaseType(DCEngine::MouseButton, Zilch::TypeCopyMode::ReferenceType);
ZilchDeclareExternalBaseType(DCEngine::MouseWheelScrollDirection, Zilch::TypeCopyMode::ReferenceType);