/*****************************************************************************/
/*!
\file   Object.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  The base object class, from which everything else is derived ??
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

      Object(std::string name);
      Object();
      virtual ~Object();
      const std::string& Name() const;
      
      // Properties
      DCE_DEFINE_PROPERTY(std::string, ObjectName);

      #if(DCE_USE_ZILCH_INTERNAL_BINDING)
      /* Using internal binding we'll declare that this class should be registered wit ZIlch.
      This macro is only a decalration and somewhere in a single cpp file we must use
      the 'ZilchDefineBaseType' macro.
      ReferenceType means it will be allocated on the heap and alwayts referenced by handle. */
      ZilchDeclareBaseType(Object, Zilch::TypeCopyMode::ReferenceType);

      //!< Serializes the Object to JSON through Zilch.      
      template <typename ObjectHandle>
      void SerializeByType(Zilch::JsonBuilder& builder, Zilch::ExecutableState* state,
        ObjectHandle objectHandle, Zilch::BoundType* boundType);
      template <typename ObjectHandle>
      void DeserializeByType(Zilch::JsonValue* properties, Zilch::ExecutableState* state, 
                     ObjectHandle objectHandle, Zilch::BoundType* boundType);

      // Serialize the base object
      void Serialize(Zilch::JsonBuilder& builder);
      void Deserialize(Zilch::JsonValue* properties);   
      #endif

      DCE_DEFINE_PROPERTY(unsigned int, ObjectID);

    protected:
      std::string ObjectName;
      static unsigned int ObjectsCreated;
      unsigned int ObjectID;
      Object* ObjectOwner; //!< Should this be a smart pointer?      

  private:

  };

  using ObjectPtr = Object*;

  /*===================*
  *     Templates      *
  *====================*/

  /**************************************************************************/
  /*!
  @brief  Serializes the Object instance through Zilch.
  @param  properties The list of properties in the Object.
  @param  state A pointer to the singleton Zilch's executable state.
  @param  objectHandle A handle (Pointer/Zilch Handle) to the Object.
  @param  boundType A pointer to the Object's derived type.
  @note   This function is called by derived classes.
  @todo   Factor out the code to convert from Values!
  */
  /**************************************************************************/
  template<typename ObjectHandle>
  inline void Object::DeserializeByType(Zilch::JsonValue * properties, Zilch::ExecutableState * state,
                                ObjectHandle objectHandle, Zilch::BoundType * boundType)
  {
    // If there's no properties, there's nothing to do!
    if (properties == nullptr || boundType == nullptr)
      return;
    
    // For every 'bound' property in the object
    for (auto property : properties->OrderedMembers.all()) {
      //
      Zilch::JsonValue* value = property->Value;
      // Attemot to either get it as a 'Property' with Getter/Setter methods
      Zilch::Property* namedProperty = boundType->GetInstanceProperty(property->Key);
      
      // If the property on file could not be found on the object, ignore
      if (namedProperty == nullptr)
        continue;

      // If there is no set method
      if (namedProperty->Set == nullptr)
        continue;

      // Or as an instance field, with only a Getter method
      //if (namedProperty == nullptr)
      //  namedProperty = boundType->GetInstanceField(property->Key);
      


      
        //continue;

      // If the bound field/property does not have the 'Property' attribute, 
      // do nothing!
      //if (namedProperty != nullptr && !namedProperty->HasAttribute("Property")
      //  continue;
            
      // Construct a Zilch::Call object for invoking the set method!
      Zilch::ExceptionReport report;
      Zilch::Call call(namedProperty->Set, state);
      call.SetHandleVirtual(Zilch::Call::This, objectHandle);

      bool isValidProperty = true;

      /////////////////////
      // Deserialize by type
      /////////////////////
      // Type is a float
      if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Real)))
      {
        call.Set(0, value->AsFloat());
      }
      // Type is an int.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Integer)))
      {
        call.Set(0, value->AsInteger());
      }
      // Type is a string.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::String)))
      {
        call.Set(0, value->AsString());
      }
      // Type is a Boolean.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Boolean)))
      {
        call.Set(0, value->AsBool());
      }
      // Type is a Real2.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Real2)))
      {
        auto real2 = value->GetMember("Vec2");
        auto x = real2->GetMember("x")->AsFloat();
        auto y = real2->GetMember("y")->AsFloat();
        call.Set(0, Zilch::Real2(x,y));
      }
      // Type is a Real3.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Real3)))
      {
        auto real3 = value->GetMember("Vec3");
        auto x = real3->GetMember("x")->AsFloat();
        auto y = real3->GetMember("y")->AsFloat();
        auto z = real3->GetMember("z")->AsFloat();
        call.Set(0, Zilch::Real3(x, y, z));        
      }
      // Type is a Real4.
      else if (Zilch::Type::IsSame(namedProperty->PropertyType, ZilchTypeId(Zilch::Real4)))
      {
        auto real4 = value->GetMember("Vec4");
        auto x = real4->GetMember("x")->AsFloat();
        auto y = real4->GetMember("y")->AsFloat();
        auto z = real4->GetMember("z")->AsFloat();
        auto w = real4->GetMember("w")->AsFloat();
        call.Set(0, Zilch::Real4(x, y, z, w));        
      }
      // Type is invalid 
      else {
        isValidProperty = false;
        DCTrace << "Object::Deserialize - Attempting to read property: " << property->Key.c_str()
                << " from " << boundType->Name.c_str() << " as a "
                << namedProperty->PropertyType->ToString().c_str()
                << " which is not supported! \n";
      }

      // If the property was valid
      if (isValidProperty)
        call.Invoke(report);
      else {
        DCTrace << "You have likely removed " << property->Key.c_str()
                << " from " << boundType->Name.c_str()
                << " but are still serializing it. \n";
      }
    }
  }

  /**************************************************************************/
  /*!
  @brief  Serializes the Object instance by type through Zilch.
  @param  builder A reference to the JSON builder which will contain the
          deserialized data.
  @param  state A pointer to the singleton Zilch's executable state.
  @param  objectHandle A handle (Pointer/Zilch Handle) to the Object.
  @param  boundType A pointer to the Object's derived type.
  @note
  */
  /**************************************************************************/
  template<typename ObjectHandle>
  inline void Object::SerializeByType(Zilch::JsonBuilder & builder, Zilch::ExecutableState * state, 
                                      ObjectHandle objectHandle, Zilch::BoundType * boundType)
  {

    Zilch::PropertyArray& properties = boundType->AllProperties;
    Zilch::ExceptionReport report;

    for (auto property : properties) {

      // If the bound field/property does not have the 'Property' attribute, 
      // do nothing!
      //if (!property->HasAttribute("Property")
      //  continue;

      // Create a key for the builder
      auto pname = property->Name;
      builder.Key(property->Name);
      //builder.Begin(Zilch::JsonType::Object);      
      

      // Set up the 'getter' method so that we can serialize the value
      Zilch::Call call(property->Get, state);
      call.SetHandleVirtual(Zilch::Call::This, objectHandle);
      call.Invoke(report);
      
      /////////////////////
      // Serialize by type
      /////////////////////

      // Property: Real (float)
      if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real)))
      {
        builder.Value(call.Get<Zilch::Real>(Zilch::Call::Return));
      }
      // Property: Integer
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Integer)))
      {
        builder.Value(call.Get<Zilch::Integer>(Zilch::Call::Return));
      }
      // Property: String
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::String)))
      {
        builder.Value(call.Get<Zilch::String>(Zilch::Call::Return));
      }
      // Property: Boolean
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Boolean)))
      {
        builder.Value(call.Get<Zilch::Boolean>(Zilch::Call::Return));
      }
      // Property: Real2
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real2)))
      {
        auto value = call.Get<Zilch::Real2>(Zilch::Call::Return);
        //Real2AsValue(builder, value);
        // Serialize a Vec2 Object
        builder.Begin(Zilch::JsonType::Object);
        {
          builder.Key("Vec2");
          builder.Begin(Zilch::JsonType::Object);
          {
            builder.Key("x");
            builder.Value(value.x);
            builder.Key("y");
            builder.Value(value.y);
          }
          builder.End();
        }
        builder.End();
      }
      // Property: Real3
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real3)))
      {
        auto value = call.Get<Zilch::Real3>(Zilch::Call::Return);
        //Real3AsValue(builder, value);
        // Serialize a Vec3 Object
        builder.Begin(Zilch::JsonType::Object);
        {       
          builder.Key("Vec3");
          builder.Begin(Zilch::JsonType::Object);
          {
            builder.Key("x");
            builder.Value(value.x);
            builder.Key("y");
            builder.Value(value.y);
            builder.Key("z");
            builder.Value(value.z);
          }
          builder.End();
        }
        builder.End();
      }
      // Property: Real4
      else if (Zilch::Type::IsSame(property->PropertyType, ZilchTypeId(Zilch::Real4)))
      {
        auto value = call.Get<Zilch::Real4>(Zilch::Call::Return);
        //Real4AsValue(builder, value);
        builder.Begin(Zilch::JsonType::Object);
        {
          builder.Key("Vec4");
          builder.Begin(Zilch::JsonType::Object);
          {
            builder.Key("x");
            builder.Value(value.x);
            builder.Key("y");
            builder.Value(value.y);
            builder.Key("z");
            builder.Value(value.z);
            builder.Key("w");
            builder.Value(value.w);
          }
          builder.End();
        }
        builder.End();
        
      }
    }    

  }

}