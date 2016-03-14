/******************************************************************************/
/*!
@file	  ZilchEvent.cpp
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/13/2016
@brief
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ZilchEvent.h"
#include "../Binding/CoreBinding.h"
#include "../Binding/CoreBindingTypes.h"

namespace DCEngine {

  ZilchDefineType(ZilchEvent, "ZilchEvent", DCEngineCore, builder, type)
  {
    //DCE_BINDING_SET_HANDLE_TYPE_POINTER;
    ZilchBindConstructor(builder, type, ZilchEvent, ZilchNoNames);
    ZilchBindConstructor(builder, type, ZilchEvent, "name", std::string);
    ZilchBindDestructor(builder, type, ZilchEvent);
  }

  ZilchEvent::ZilchEvent() : Event("ZilchEvent")
  {
  }

  ZilchEvent::ZilchEvent(std::string name) : Event(name)
  {
  }

}