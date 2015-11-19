#pragma once
#include "../../ProjectComponent.h"

namespace DCEngine {

	class Transform;
	class Sprite;
	class CameraController : public Component {

	public:
		Transform* TransformRef;
		Sprite* SpriteRef;
		GameObject* PlayerRef;
		Real InterpolationSpeed = 0.04f; //percent of the way to interpolate per frame from 0-1

		CameraController(Entity& owner) : Component(std::string("CameraController"), owner) {}
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void OnMouseDownEvent(Events::MouseDown* event);
		void OnMouseUpEvent(Events::MouseUp* event);
		void CameraController::OnLogicUpdateEvent(Events::LogicUpdate * event);

	private:
		void PrintTranslation();
	};

}