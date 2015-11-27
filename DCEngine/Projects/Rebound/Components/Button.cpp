/*****************************************************************************/
/*!
@file   Button.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "Button.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(Button, "Button", DCEngineCore, builder, type) {
		//// Constructor / Destructor
		//ZilchBindConstructor(builder, type, Button, "owner", Entity&);
		//ZilchBindDestructor(builder, type, Button);
		//// Properties
		//ZilchBindProperty(builder, type, &Transform::getTranslation, &Transform::setTranslation, "Translation");
		//ZilchBindProperty(builder, type, &Transform::getRotation, &Transform::setRotation, "Rotation");
		//ZilchBindProperty(builder, type, &Transform::getScale, &Transform::setScale, "Scale");
	}
#endif


	void Button::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		//Connect(Daisy->getMouse(), Events::MouseDown, Button::OnMouseDownEvent);
		//Connect(Daisy->getMouse(), Events::MouseUp, Button::OnMouseUpEvent);
		//Connect(SpaceRef, Events::LogicUpdate, Button::OnLogicUpdateEvent);
		Connect(gameObj, Events::CollisionStarted, Button::OnCollisionStartedEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
	}

	void Button::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void Button::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		if (event->OtherObject->getComponent<BallController>())
		{
			auto ball = SpaceRef->FindObjectByName("Basketball");
			if (ball)
			{
				auto wall = SpaceRef->FindObjectByName("Door");
				if (wall)
				{
					wall->getComponent<Transform>()->setTranslation(Vec3(0, -100, 0));
				}
			}
		}
	}

	void Button::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void Button::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void Button::OnKeyDownEvent(Events::KeyDown * event)
	{
	}

	void Button::OnKeyUpEvent(Events::KeyDown * event)
	{
	}

	void Button::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{

	}


}