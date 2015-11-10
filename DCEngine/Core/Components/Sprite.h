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

  namespace Systems {
    class Graphics;
  }

	class Transform;
	class Sprite : public Component {
	public:
    friend class Graphics;

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(Sprite, Component);
    #endif

    /* Property methods */
    bool getVisible() const;
    void setVisible(bool);
    Vec4 getColor() const;
    void setColor(Vec4);
    String getSpriteSource() const;
    void setSpriteSource(String);
    bool getFlipX() const;
    void setFlipX(bool);
    bool getFlipY() const;
    void setFlipY(bool);
    bool getAnimationActive() const;
    void setAnimationActive(bool);
    Real getAnimationSpeed() const;
    void setAnimationSpeed(Real);
    

    /* Variables */
		String SpriteSource = "Square";
		bool Visible = true;
		Vec4 Color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
		BlendModeType BlendMode = BlendModeType::Alpha;
		bool FlipX = false;
		bool FlipY = false;
    //Animation subsystem
		bool AnimationActive = false;
		bool HaveAnimation = false;
		float AnimationSpeed = 1;
		int StartColumn = 0;//the number of first frame is 0
		int StartRow = 0;//the number of first row is 0
		int CurrentColumn = 0;
		int CurrentRow = 0;
		void IncreaseAnimationCounter(float dt);
		//return 1, speed is not 0. return 0, speed is 0
		int UpdateAnimationSpeed(void);
		//reutrn 1 if it is the time to goto next frame, else 0
		bool CheckAnimationIntialized(void);
		float GetAnimationSpeedFPSCounter(void);
		float GetAnimationSpeedFPS(void);
		void ResetSpeedCounter(void);
		void SetColorUsing255(Vec3 newColor);

    Sprite(Entity& owner);
    void Initialize();
    void UpdateSprite();
    Transform* TransformComponent;

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