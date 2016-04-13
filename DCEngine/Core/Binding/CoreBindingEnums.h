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

#include "../Resources/CollisionTable.h"

ZilchDeclareExternalBaseType(DCEngine::Direction, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::Keys, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::StopMode, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::MouseButton, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::MouseWheelScrollDirection, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::Ease, Zilch::TypeCopyMode::Enum::ValueType);
ZilchDeclareExternalBaseType(DCEngine::CollisionFlag, Zilch::TypeCopyMode::Enum::ValueType);
