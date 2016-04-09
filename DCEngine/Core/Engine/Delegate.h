/*****************************************************************************/
/*!
@file      Delegate.h
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      1/17/2015
@brief     A delegate is...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  /**************************************************************************/
  /*!
  @class Delegate Base class for delegates.
  */
  /**************************************************************************/
  class Delegate {
  public:
    Delegate() {}
    virtual ~Delegate() {}
    virtual bool Call() = 0;
  };

  template <typename Class>
  class MemberFunctionDelegate : public Delegate {
  public:
    typedef void(Class::*MemFunc)();
    MemFunc FuncPtr;
    Class* ClassInstance;

    MemberFunctionDelegate(Class* instance, MemFunc funcPtr) 
          : ClassInstance(instance), FuncPtr(funcPtr) {}

    virtual bool Call() {
      (ClassInstance->*FuncPtr)();
      return true;
    }
  };


}