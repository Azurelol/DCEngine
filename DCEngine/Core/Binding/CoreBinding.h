/******************************************************************************/
/*!
@file   CoreBinding.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/07/2015
@brief  This file includes the Zilch binding header. This header is included
        by all the classes that want to be added to the library.
*/
/******************************************************************************/
#pragma once
//#include <ZILCH\Zilch.hpp>

/////////////////////////
// Binding Configuration
/////////////////////////
#define DCE_USE_ZILCH_INTERNAL_BINDING 1
#define DCE_BINDING_OBJECT_CLASSES_INTERNALLY 0

// What type of handle manager to use for components. For now we will be 
// constructing components internally so we use pointers.
#define DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE \
type->HandleManager = ZilchManagerId(Zilch::PointerManager)



namespace DCEngine {
  // Declare internal library singleton
  ZilchDeclareStaticLibrary(DCEngineCore);

}
