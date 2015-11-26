#pragma once

namespace DCEngine {

  // What type of handle manager to use for components. 
  // For now we will be constructing components internally so we use pointers.
  // This needs to be added at the top of every component definition.
  #define REBOUND_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE \
  type->HandleManager = ZilchManagerId(Zilch::PointerManager)

  // Declare the Rebound static library
  ZilchDeclareStaticLibrary(Rebound);

  // Add the components to the factory
  void ReboundComponentsAddToFactory();

}
