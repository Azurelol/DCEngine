#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class Sprite;
	class ChargeBar : public Component {

	public:
		Transform* TransformRef;
		Sprite* SpriteRef;
		GameObject* BallRef;
		Real ScaleX = 2.5;

		ChargeBar(Entity& owner) : Component(std::string("ChargeBar"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void ChargeBar::OnLogicUpdateEvent(Events::LogicUpdate * event);

	private:
		void PrintTranslation();
	};

}