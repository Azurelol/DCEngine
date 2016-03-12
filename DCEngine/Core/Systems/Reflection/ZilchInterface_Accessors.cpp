/******************************************************************************/
/*!
@file   ZilchInterface_Accessors.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   3/11/2016
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "ZilchInterface.h"

namespace DCEngine {
  namespace Systems {

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the Library.
    @return The current Script Library Reference for the interface.
    */
    /**************************************************************************/
    Zilch::LibraryRef ZilchInterface::getLibrary()
    {
      return ScriptLibrary;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the Boundtype of the specified object
    from the specified Zilch library.
    @param  typeName The name of the type.
    @param  library A reference to the library.
    @return A pointer to the bound type.
    */
    /**************************************************************************/
    Zilch::BoundType * ZilchInterface::getBoundType(std::string typeName, Zilch::LibraryRef library)
    {
      Zilch::BoundType* type = library->BoundTypes.findValue(typeName.c_str(), nullptr);
      ErrorIf(type == nullptr, std::string("Failed to find a Zilch type named '" + typeName + "'").c_str());
      return type;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the specified function on the object.
    @param  typeName The name of the function.
    @param  type The BoundType.
    @param  parameters An array specifying the arguments of the function.
    @param  returnType The return type of the function.
    @return A pointer to the function.
    */
    /**************************************************************************/
    Zilch::Function * ZilchInterface::getFunction(std::string name, Zilch::BoundType* type,
      const Zilch::Array<Zilch::Type*>& parameters,
      Zilch::Type* returnType,
      Zilch::FindMemberOptions::Flags options,
      bool ErrorOn)

    {
      Zilch::Function* function = type->FindFunction(name.c_str(),
        parameters,
        returnType,
        options);

      auto typeName = type->Name.c_str();
      ErrorIf(function == nullptr, std::string("Failed to find a function named '" + name + "' on" + typeName).c_str());
      return function;
    }

    /**************************************************************************/
    /*!
    @brief  Grabs a reference to the specified instance field of the specified
    BoundType.
    @param  typeName The name of the instnace.
    @param  type The BoundType.
    @return A pointer to the function.
    */
    /**************************************************************************/
    Zilch::Field * ZilchInterface::getInstanceField(std::string name, Zilch::BoundType* type)
    {
      Zilch::Field* field = type->InstanceFields.findValue(name.c_str(), nullptr);
      auto typeName = type->Name.c_str();
      ErrorIf(field == nullptr, std::string("Failed to find '" + name + "' on " + typeName).c_str());
      return field;
    }

    /*!************************************************************************\
    @brief  Retrieves an attribute from a property by name.
    @param property A pointer to the property.
    @param attributeName The name of the attribute which to retrieve.
    @return A pointer to the attribute.
    \**************************************************************************/
    Zilch::Attribute * ZilchInterface::getAttribute(Zilch::Property * property, std::string attributeName)
    {
      for (auto& attribute : property->Attributes.all()) {
        if (attribute.Name == attributeName.c_str())
          return &attribute;
      }
    }

    /*!************************************************************************\
    @brief  Gets all the currently bound types.
    @return A vector containing all the bound types.
    \**************************************************************************/
    std::vector<Zilch::BoundType*> ZilchInterface::GetTypes()
    {
      std::vector<Zilch::BoundType*> allTypes;

      // Loop through every library
      for (auto library : this->Dependencies.all()) {
        // Grab a container of all the bound types in the library
        auto types = library->BoundTypes.all();
        // For every type in the library
        ZilchForEach(auto type, types) {

        }
      }
      return std::vector<Zilch::BoundType*>();
    }

  }
}