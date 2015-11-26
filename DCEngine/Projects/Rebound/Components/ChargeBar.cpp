/******************************************************************************/
/*!
@file   ChargeBar.cpp
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Responsible for making the charge bar above the player connect to the
		ball and change size/color.
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/

#include "ChargeBar.h"
#include "../../CoreComponents.h"

namespace DCEngine {

	void ChargeBar::Initialize()
	{
		auto gameObj = dynamic_cast<GameObject*>(Owner());
		Connect(Daisy->getMouse(), Events::MouseDown, ChargeBar::OnMouseDownEvent);
		Connect(Daisy->getMouse(), Events::MouseUp, ChargeBar::OnMouseUpEvent);
		Connect(SpaceRef, Events::LogicUpdate, ChargeBar::OnLogicUpdateEvent);
		TransformRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Transform>(); 
		SpriteRef = dynamic_cast<GameObject*>(ObjectOwner)->getComponent<Sprite>();
		BallRef = SpaceRef->FindObjectByName("Basketball");
	}

	void ChargeBar::Serialize(Json::Value & root)
	{
	}

	void ChargeBar::Deserialize(Json::Value & root)
	{
	}

	void ChargeBar::OnMouseDownEvent(Events::MouseDown * event)
	{

	}

	void ChargeBar::OnMouseUpEvent(Events::MouseUp * event)
	{

	}

	void ChargeBar::OnLogicUpdateEvent(Events::LogicUpdate * event)
	{
		auto curr = BallRef->getComponent<BallController>()->CurrentCharge;
		auto max = BallRef->getComponent<BallController>()->MaxCharge;
		auto val = curr / max;
		SpriteRef->Color = Vec4(1, 1 - val, 1 -val, 1);
		TransformRef->Scale = Vec3(ScaleX * val, TransformRef->Scale.y, TransformRef->Scale.z);
	}



	void ChargeBar::PrintTranslation()
	{
		//DCTrace << Owner()->Name() << "::Transform.Translation(" << TransformRef->Translation.x
		//	<< ", " << TransformRef->Translation.y
		//	<< ", " << TransformRef->Translation.z << ")\n";
	}

	/**************************************************************************/
	/*!
	@brief Provides the definition of this class to Zilch.
	@note This can only go in the translational unit (.cpp)
	*/
	/**************************************************************************/
	#if(DCE_USE_ZILCH_INTERNAL_BINDING)
	ZilchDefineType(ChargeBar, "ChargeBar", DCEngineCore, builder, type) {
		// Constructor / Destructor
		ZilchBindConstructor(builder, type, Transform, "owner", Entity&);
		ZilchBindDestructor(builder, type, Transform);
		// Properties
	}
	#endif
}