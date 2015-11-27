/*****************************************************************************/
/*!
\file   TimeSpace.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/14/2015
\brief  This component controls time for a Space.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class TimeSpace : public Component {
  public:

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(TimeSpace, Component);
    #endif

    // Property Methods
    DCE_DEFINE_PROPERTY(bool, Paused);
    DCE_DEFINE_PROPERTY(float, TimeScale);
    DCE_DEFINE_PROPERTY(int, StepCount);

    TimeSpace(Entity& owner);
    void Initialize();

  private:
    // Properties
    bool Paused = false; // If the time space is paused then we cease sending out LogicUpdate events.
    float TimeScale = 1; // Scale the speed of time for interesting effects like bullet time..
    int StepCount = 1; // Causes the engine to update this space 'n' times

    void OnLogicUpdate(Events::LogicUpdate* updateEvent);

  };

}
