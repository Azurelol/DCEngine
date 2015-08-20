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

#include <cassert>
#include <string>
#include <memory>

// Every object can access 'trace'
#include "..\Debug\Debug.h"

namespace DCEngine {

  class Object {
    public:
      Object(std::string name) { _name = name; }
      Object() { _name = "Object"; }

      std::string Name() { return _name; }

    protected:
      std::string _name;

  };

  using ObjectPtr = std::shared_ptr<Object>;

}