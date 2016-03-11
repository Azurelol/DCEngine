/******************************************************************************/
/*!
\file   EventDelegate.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/9/2015
\brief  A delegate is a class that wraps a pointer or reference to an object
instance, a member method of that object's class to be called on that
object instance, and provides a method to trigger that call.
\note   I am too inexperienced a programmer to have been able to come up
with the implementation on my own. Thus, I researched and ended up
reading through several implementations:

http://allenchou.net/2012/04/easy-c-delegates/
http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
http://www.codeguru.com/cpp/cpp/cpp_mfc/pointers/article.php/c4135/DELEGATES-and-C.htm
http://codereview.stackexchange.com/questions/14730/impossibly-fast-delegate-in-c11

I also got a lot of help from one of my mentors, Arend Danielek, after
he walked me through the way he implemented the event system in his Junior
game, "Coffee Break".

9/11/2015: I finally finished the delegate implementation, in no small measure
thanks to Gab and Arend.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.


*/
/******************************************************************************/
#include "EventDelegate.h"
#include "Delegate.h"
#include "../Objects/Event.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief  Calls the member function given an event.
  @param  A pointer to the event object.
  @note   If the Object instance is no longer valid (such as if the object
  was destroyed, this will return false)
  */
  /**************************************************************************/
  bool EventZilchFunctionDelegate::Call(Event * event)
  {
    Zilch::ExceptionReport report;
    Zilch::Call call(Delegate, State);
    //call.DisableParameterChecks();
    call.Set(0, event);
    call.Invoke(report);
    return true;
  }

  Object * EventZilchFunctionDelegate::GetObserver()
  {
    return reinterpret_cast<Object*>(Delegate.ThisHandle.Dereference());
  }

}
