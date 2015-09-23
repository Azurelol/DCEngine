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
#include "..\Debug\Debug.h" // Every object can access 'trace'

namespace DCEngine {

  //using ObjectPtr = std::shared_ptr<Object>;

  // Foward-declarations
  class Space;
  class GameSession;

  class Object {
    friend class GameSession;
    friend class Space;

    public:
      Object(std::string name) { _name = name; }
      Object() { _name = "Object"; }
      virtual ~Object() {}; // Let's not leak memory
	    
      //virtual void Owner() = 0; // Every class needs to implement its owner pointer.

      const std::string& Name() const { return _name; } 

    protected:
      std::string _name;
      int RuntimeId;
      Object* owner_; //!< Should this be a smart pointer?
      //std::shared_ptr<Object> _owner;

  private:
    //Object() = delete; // No default construction

  };

  using ObjectPtr = std::shared_ptr<Object>;

}