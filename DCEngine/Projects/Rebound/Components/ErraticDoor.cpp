/*****************************************************************************/
/*!
@file   ErraticDoor.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "ErraticDoor.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(ErraticDoor, "ErraticDoor", Rebound, builder, type) {
		DCE_BINDING_DEFINE_PROPERTY(ErraticDoor, Timer);
	}
#endif


	void ErraticDoor::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		//Connect(Daisy->getKeyboard(), Events::KeyDown, ErraticDoor::OnKeyDownEvent);
		//Connect(Daisy->getKeyboard(), Events::KeyUp, ErraticDoor::OnKeyUpEvent);
		//Connect(Daisy->getMouse(), Events::MouseDown, ErraticDoor::OnMouseDownEvent);
		//Connect(Daisy->getMouse(), Events::MouseUp, ErraticDoor::OnMouseUpEvent);
		//Connect(gameObj, Events::CollisionStarted, ErraticDoor::OnCollisionStartedEvent);
		//Connect(gameObj, Events::CollisionEnded, ErraticDoor::OnCollisionEndedEvent);
		//Connect(SpaceRef, Events::LogicUpdate, ErraticDoor::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		//RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		MTLRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<MoveToLocation>();
		RandomDelay = (rand() % 10 + 1) * 2;
	}

	void ErraticDoor::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void ErraticDoor::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
	}

	void ErraticDoor::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void ErraticDoor::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void ErraticDoor::OnKeyDownEvent(Events::KeyDown * event)
	{
	}

	void ErraticDoor::OnKeyUpEvent(Events::KeyDown * event)
	{
	}

	void ErraticDoor::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		if (MTLRef->getMoving() == true)
			return;

		Timer += event->Dt;
		if (Timer > RandomDelay)
		{
			MTLRef->setMoving(true);
		}
	}


}