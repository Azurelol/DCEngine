/*****************************************************************************/
/*!
\file   TimeSpace.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/14/2015
\brief  This component controls time for a Space.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class TimeSpace : public Component {
  public:

    //DCE_DECLARE_PROPERTY(bool, Paused);
    
    // Property Methods
    bool getPaused() const;
    void setPaused(bool);
    float getTimeScale() const;
    void setTimeScale(float);
    int getStepCount() const;
    void setStepCount(int);   

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
