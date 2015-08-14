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
#include <string>
#include <memory>

namespace DCEngine {

  class Object {
    public:
      Object(std::string name) { _name = name; }
      Object() { _name = "Object"; }

    protected:
      std::string _name;

  };

  using ObjectPtr = std::shared_ptr<Object>;

}