/******************************************************************************/
/*!
@file   Sprite.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/19/2015
@brief  The Sprite component allows the graphical representation of this object
in the world space through the drawing of sprites.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Resources/SpriteSource.h"

//Note: 1. BlendMode and SpriteSource 0%
//      2. Visible, Color, FlipX/Y need Graphics Pipeline. Local prgress done.

namespace DCEngine {

	enum class BlendModeType {
		Alpha,
		Additive,
		Multiply,
		MultiplyAdd,
		ColorAdditive,
		None,
	};

	class Transform;
	class Sprite : public Component {
	public:

		Transform* TransformComponent;
		String SpriteSource = "square";
		bool Visible = true;
		Real4 Color = Real4(1.0f, 1.0f, 1.0f, 1.0f);
		BlendModeType BlendMode = BlendModeType::Alpha;
		bool FlipX = false;
		bool FlipY = false;

		Sprite(Entity& owner);
		void Initialize();
		virtual void Serialize(Json::Value& root);
		virtual void Deserialize(Json::Value& root);
		void UpdateSprite();

		//Animation subsystem
		bool AnimationActive = false;
		bool HaveAnimation = false;
		float AnimationSpeed = 1;
		int StartFrame = 0;//the number of first frame is 0
		int CurrentFrame = 0;
		void IncreaseAnimationCounter(float dt);
		//return 1, speed is not 0. return 0, speed is 0
		int UpdateAnimationSpeed(void);
		//reutrn 1 if it is the time to goto next frame, else 0
		bool CheckAnimationIntialized(void);
		float GetAnimationSpeedFPSCounter(void);
		float GetAnimationSpeedFPS(void);
		void ResetSpeedCounter(void);
		void SetColorUsing255(Real3 newColor);

	private:

		bool XFlipped;
		bool YFlipped;
		glm::mat4 FlipMatrix;

		//Update
		void UpdateFlip();
		//Animation subsystem
		bool AnimationInitialized = false;
		float AnimationSpeedFPS = 0;
		float AnimationSpeedFPSCounter = 0;
	};

}
