/******************************************************************************/
/*!
@file   Grunt.h
@author Jason Jorgenson
@par    email: Jason.Jorgenson\@digipen.edu
@date   1/16/2016
@brief  An AI that patrols on a set route until the player enter's it's area.
        Then it will attack until the player leaves.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Grunt.h"
#include "../../../CoreComponents.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(Grunt, "Grunt", Rebound, builder, type) {
      DCE_BINDING_DEFINE_PROPERTY(Grunt, PlayerName);
    }
#endif


    void Grunt::Initialize()
    {
      auto gameObj = dynamic_cast<GameObject*>(Owner());
      TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Transform>(); 
      SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Components::Sprite>();
    
      *stateMachine = StateMachine<Grunt>(this);
      //stateMachine->ChangeState(new GruntIdleState<Grunt>());

      player = SpaceRef->FindObjectByName(PlayerName);
    }

    void Grunt::OnLogicUpdateEvent(Events::LogicUpdate * event)
    {
      stateMachine->Update();
    }

    void Grunt::OnCollisionStartedEvent(Events::CollisionStarted * event)
    {
    }

    void Grunt::OnCollisionEndedEvent(Events::CollisionEnded * event)
    {
    }



  }
}