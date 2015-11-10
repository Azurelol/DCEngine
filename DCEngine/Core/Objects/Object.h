/*****************************************************************************/
/*!
\file   Object.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   8/5/2015
\brief  The base object class, from which everything else is derived ??
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

      #if(DCE_USE_ZILCH_INTERNAL_BINDING)
      /* Using internal binding we'll declare that this class should be registered wit ZIlch.
      This macro is only a decalration and somewhere in a single cpp file we must use
      the 'ZilchDefineBaseType' macro.
      ReferenceType means it will be allocated on the heap and alwayts referenced by handle. */
      ZilchDeclareBaseType(Object, Zilch::TypeCopyMode::ReferenceType);
      #endif

    protected:
      std::string ObjectName;
      static unsigned int ObjectsCreated;
      const unsigned int ObjectID;
      Object* ObjectOwner; //!< Should this be a smart pointer?      

  private:

  };

  using ObjectPtr = std::shared_ptr<Object>;

}