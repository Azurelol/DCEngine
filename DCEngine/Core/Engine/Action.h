/*****************************************************************************/
/*!
@file   Action.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   8/6/2015
@brief  An action
        Reference: http://gamedevelopment.tutsplus.com/tutorials/the-action-list-data-structure-good-for-ui-ai-animations-and-more--gamedev-9264
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

#include "ActionList.h"
#include "Delegate.h"

namespace DCEngine {

  class ActionSet {

  };
  
  enum class Ease {
    Linear,
    QuadIn,
    QuadInOut,
    QuadOut,
    SinIn,
    SinInOut,
    SinOut,
  };

  class Action {
  public:
    // Constructs an action sequence, adding it to 
    static ActionSet Sequence(ActionSet& actions);    
    static void Call(ActionSet& set, Delegate& fn);
    static void Delay(ActionSet& set, Real duration);
    template <typename Property, typename EndValue>
    static void Property(ActionSequence& seq, Property prty, EndValue val, Ease ease);

  private:

  };

  template<typename Property, typename EndValue>
  inline void Action::Property(ActionSequence & seq, Property prty, EndValue val, Ease ease)
  {

  }

}