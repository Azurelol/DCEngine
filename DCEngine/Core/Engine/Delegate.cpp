/*****************************************************************************/
/*!
@file      Delegate.cpp
@author    Christian Sagel
@par       email: c.sagel\@digipen.edu
@date      3/25/2016
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#include "Delegate.h"

namespace DCEngine {

  bool ZilchMemberFunctionDelegate::Call()
  {
    Zilch::ExceptionReport report;
    Zilch::Call call(Delegate, State);
    call.DisableParameterChecks();
    call.Invoke(report);
    return false;
  }

}