/*****************************************************************************/
/*!
@file   TutorialTextLogic.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/19/2015
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "TutorialTextLogic.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(TutorialTextLogic, "TutorialTextLogic", Rebound, builder, type) {
		DCE_BINDING_DEFINE_PROPERTY(TutorialTextLogic, Timer);
		DCE_BINDING_DEFINE_PROPERTY(TutorialTextLogic, Type);
	}
#endif


	void TutorialTextLogic::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getKeyboard(), Events::KeyDown, TutorialTextLogic::OnKeyDownEvent);
		//Connect(Daisy->getKeyboard(), Events::KeyUp, TutorialTextLogic::OnKeyUpEvent);
		//Connect(Daisy->getMouse(), Events::MouseDown, TutorialTextLogic::OnMouseDownEvent);
		//Connect(Daisy->getMouse(), Events::MouseUp, TutorialTextLogic::OnMouseUpEvent);
		//Connect(gameObj, Events::CollisionStarted, TutorialTextLogic::OnCollisionStartedEvent);
		//Connect(gameObj, Events::CollisionEnded, TutorialTextLogic::OnCollisionEndedEvent);
		Connect(SpaceRef, Events::LogicUpdate, TutorialTextLogic::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); // ew
																						  //RigidBodyRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<RigidBody>();
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		//SpriteRef->setVisible(false);
		SpriteRef->Color = Vec4(1, 1, 1, 0);
	}

	void TutorialTextLogic::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void TutorialTextLogic::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
	}

	void TutorialTextLogic::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{
	}

	void TutorialTextLogic::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void TutorialTextLogic::OnKeyDownEvent(Events::KeyDown * event)
	{
		if (Type == 0)
		{
			if (event->Key == Keys::A || event->Key == Keys::D)
			{
				Timer = 0;
			}
		}
		else
		{
			if (event->Key == Keys::W || event->Key == Keys::Space)
			{
				Timer = 0;
			}
		}
	}

	void TutorialTextLogic::OnKeyUpEvent(Events::KeyDown * event)
	{
	}

	void TutorialTextLogic::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		Timer += event->Dt;
		if (Timer < 5)
		{
			SpriteRef->Color = Vec4(1, 1, 1, 0);
			//DCTrace << "visible\n";
		}
		else
		{
			SpriteRef->Color = Vec4(1, 1, 1, 1);
			//DCTrace << "not visible\n";
		}
	}


}