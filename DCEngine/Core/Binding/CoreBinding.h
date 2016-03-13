/******************************************************************************/
/*!
@file   CoreBinding.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/07/2015
@brief  This file includes the Zilch binding header. This header is included
        by all the classes that want to be added to the library.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
//#include <ZILCH\Zilch.hpp>

/////////////////////////
// Binding Configuration
/////////////////////////
#define DCE_USE_ZILCH_INTERNAL_BINDING 1
#define DCE_BINDING_OBJECT_CLASSES_INTERNALLY 0


#define DCE_BINDING_INTERNAL_COMPONENT_SET_ATTRIBUTE_RESOURCE \
Zilch::Attribute resourceAttribute; \
resourceAttribute.Name = "Resource"



#define DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(resource) \
Zilch::Attribute attributeResource;                     \
attributeResource.Name = "Resource";                    \
Zilch::Attribute attribute##resource;                   \
attribute##resource.Name = "" #resource

#define DCE_BINDING_SET_RESOURCE_ATTRIBUTE(resource)  \


/*
Zilch::Attribute resource##Attribute; \
resource##Attribute.Name = "" #resource
*/

// What type of handle manager to use for components. For now we will be 
// constructing components internally so we use pointers.
#define DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE \
type->HandleManager = ZilchManagerId(Zilch::PointerManager)

// What type of handle manager to use for resources. For now we will be 
// constructing components internally so we use pointers.
#define DCE_BINDING_SET_HANDLE_TYPE_POINTER \
type->HandleManager = ZilchManagerId(Zilch::PointerManager)


namespace DCEngine {
  // Declare internal library singleton
  ZilchDeclareStaticLibrary(DCEngineCore);

}
