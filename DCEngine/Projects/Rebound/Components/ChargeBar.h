/******************************************************************************/
/*!
@file   ChargeBar.h
@author Connor Tilley
@par    email: connor.tilley\@digipen.edu
@date   11/23/2015
@brief  Header for ChargeBar
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ReboundComponent.h"

#define ChargeBarTraceOn 0 // @todo Use a static bool instead

namespace DCEngine {

	class Transform;
	class Sprite;
	class ChargeBar : public Component {

	public:
		Transform* TransformRef;
		Sprite* SpriteRef;
		GameObject* BallRef;
		Real ScaleX = 2.5;

    DCE_DEFINE_PROPERTY(Real, ScaleX);

		ChargeBar(Entity& owner) : Component(std::string("ChargeBar"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void ChargeBar::OnLogicUpdateEvent(Events::LogicUpdate * event);

		#if (DCE_USE_ZILCH_INTERNAL_BINDING)
				ZilchDeclareDerivedType(ChargeBar, Component);
		#endif

	private:
		void PrintTranslation();
	};

}