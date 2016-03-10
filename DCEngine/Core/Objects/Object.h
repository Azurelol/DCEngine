/*****************************************************************************/
/*!
\file   Object.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  The base object class, from which everything else is derived.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

// Libraries
#include <cassert>
// Headers
#include "../Engine/Types.h" // Every object has access to the base types
#include "..\Debug\Debug.h" // Every object can access 'trace'

#include "../Binding/CoreBinding.h"
#include "../Binding/CoreBindingTypes.h"
#include "../Binding/CoreBindingObjects.h"

namespace DCEngine {

  class Space;
  class GameSession;

  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  //ZilchDeclareRedirectType(std::string, Zilch::String);
  class Object : public Zilch::IZilchObject {
  #else
  class Object  {
  #endif

    friend class GameSession;
    friend class Space;

    public:

      // Main methods
      ZilchDeclareBaseType(Object, Zilch::TypeCopyMode::ReferenceType);
      Object(std::string name);
      Object();
      virtual ~Object();
      const std::string& Name() const;      
      // Properties
      DCE_DEFINE_PROPERTY(std::string, ObjectName);
      DCE_DEFINE_PROPERTY(unsigned int, ObjectID);
      // Main serialization methods
      template <typename ObjectPtr>
      void SerializeByType(Zilch::JsonBuilder& builder, Zilch::ExecutableState* state,
        Zilch::BoundType* boundType, ObjectPtr objectHandle, Zilch::Handle = Zilch::Handle());
      template <typename ObjectPtr>
      void DeserializeByType(Zilch::JsonValue* properties, Zilch::ExecutableState* state, 
        Zilch::BoundType* boundType, ObjectPtr objectHandle, Zilch::Handle = Zilch::Handle());
      // Serialize the base object
      void Serialize(Zilch::JsonBuilder& builder);
      void Deserialize(Zilch::JsonValue* properties);   


    protected:
      std::string ObjectName;
      static unsigned int ObjectsCreated;
      unsigned int ObjectID;
            

  private:
  };

  using ObjectPtr = Object*;
  using ObjectContainer = std::vector<ObjectPtr>;
 
}

// Templates
#include "Object.hpp"