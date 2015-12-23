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
  namespace Components
  {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(TimeSpace, "TimeSpace", DCEngineCore, builder, type) {
      //DCE_BINDING_INTERNAL_COMPONENT_SET_HANDLE_TYPE;
      // Constructor / Destructor
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(TimeSpace);
      //ZilchBindConstructor(builder, type, TimeSpace, "owner", Entity&);
      //ZilchBindDestructor(builder, type, TimeSpace);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(TimeSpace, Paused);
      DCE_BINDING_DEFINE_PROPERTY(TimeSpace, TimeScale);
      DCE_BINDING_DEFINE_PROPERTY(TimeSpace, StepCount);

    }
#endif

    bool TimeSpace::Pause()
    {
      Paused = !Paused;
      DCTrace << "TimeSpace::Pause - " << std::boolalpha << Paused << "\n";
      return Paused;
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

}