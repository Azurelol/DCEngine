/*****************************************************************************/
/*!
\file   TimeSpace.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   11/14/2015
\brief  This component controls time for a Space.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "TimeSpace.h"
#include "EngineReference.h"

namespace DCEngine {

  //DCE_DEFINE_PROPERTY(TimeSpace, bool, Paused);

  bool TimeSpace::getPaused() const
  {
    return this->Paused;
  }

  void TimeSpace::setPaused(bool pause)
  {
    this->Paused = pause;
  }

  float TimeSpace::getTimeScale() const
  {
    return this->TimeScale;
  }

  void TimeSpace::setTimeScale(float timeScale)
  {
    this->TimeScale = timeScale;
  }

  int TimeSpace::getStepCount() const
  {
    return this->StepCount;
  }

  void TimeSpace::setStepCount(int count)
  {
    this->StepCount = count;
  }

  /**************************************************************************/
  /*!
  @brief Default TimeSpace constructor.
  @param owner The name of the entity.
  */
  /**************************************************************************/
  TimeSpace::TimeSpace(Entity & owner) : Component(std::string("TimeSpace"), owner)
  {
  }

  /**************************************************************************/
  /*!
  @brief Initializes the TimeSpace.
  */
  /**************************************************************************/
  void TimeSpace::Initialize() {
    //Connect(SpaceRef, Events::LogicUpdate, TimeSpace::OnLogicUpdate);

  }

  /**************************************************************************/
  /*!
  @brief Updates the TimeSpace
  */
  /**************************************************************************/
  void TimeSpace::OnLogicUpdate(Events::LogicUpdate* updateEvent) {
    //Events::LogicUpdate* upcastedEvent = (Events::LogicUpdate*)updateEvent;
    //DCTrace << "TimeSpace::OnLogicUpdate - Dt: " << upcastedEvent->Dt << "\n";   

  }



}