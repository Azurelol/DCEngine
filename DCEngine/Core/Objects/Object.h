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
#include "../Engine/Types.h"
#include "../Systems/Reflection/ReflectionMeta.h" // Every object can be reflected
#include "..\Debug\Debug.h" // Every object can access 'trace'
#include "..\..\Projects\Dollhouse\ReflectionTest.h"

namespace DCEngine {

  //using ObjectPtr = std::shared_ptr<Object>;

  // Foward-declarations
  class Space;
  class GameSession;

  class Object {
    friend class GameSession;
    friend class Space;

    public:
      Object(std::string name) { ObjName = name; }
      Object() { ObjName = "Object"; }
      virtual ~Object() {}; // Let's not leak memory
	    
      //virtual void Owner() = 0; // Every class needs to implement its owner pointer.

      const std::string& Name() const { return ObjName; }

    protected:
      std::string ObjName;
      int RuntimeId;
      Object* owner_; //!< Should this be a smart pointer?
      //std::shared_ptr<Object> _owner;

  private:
    //Object() = delete; // No default construction

  };

  using ObjectPtr = std::shared_ptr<Object>;

}