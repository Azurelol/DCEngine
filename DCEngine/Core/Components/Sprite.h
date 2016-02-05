/******************************************************************************/
/*!
@file   Sprite.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/19/2015
@brief  The Sprite component allows the graphical representation of this object
in the world space through the drawing of sprites.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Graphical.h"
#include "Camera.h"

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

  namespace Components
  {
    class Transform;
    class Sprite : public Graphical {
    public:
      friend class Graphics;          
      
      DCE_COMPONENT_DECLARE_DEPENDENCIES;

      // Variables
      String SpriteSource = "square";
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
      // Properties
      DCE_DEFINE_PROPERTY(bool, Visible);
      DCE_DEFINE_PROPERTY(Vec4, Color);
      DCE_DEFINE_PROPERTY(String, SpriteSource);
      DCE_DEFINE_PROPERTY(bool, FlipX);
      DCE_DEFINE_PROPERTY(bool, FlipY);
      DCE_DEFINE_PROPERTY(bool, AnimationActive);
      DCE_DEFINE_PROPERTY(float, AnimationSpeed);
			DCE_DEFINE_PROPERTY(unsigned, DrawLayer);
      
      // Methods
      void IncreaseAnimationCounter(float dt);
      //return 1, speed is not 0. return 0, speed is 0
      int UpdateAnimationSpeed(void);
      //reutrn 1 if it is the time to goto next frame, else 0
      bool CheckAnimationIntialized(void);
      float GetAnimationSpeedFPSCounter(void);
      float GetAnimationSpeedFPS(void);
      void ResetSpeedCounter(void);
      void SetColorUsing255(Vec3 newColor);
			virtual void Update(float dt);
			virtual void Draw(Camera& camera);
			//unsigned GetDrawLayer(void);

      // Constructor
      ZilchDeclareDerivedType(Sprite, Graphical);
      Sprite(Entity& owner);
      virtual ~Sprite();
      void Initialize();
      void UpdateSprite();
      Transform* TransformComponent;
      glm::mat4 FlipMatrix;
      bool XFlipped = false;
      bool YFlipped = false;
			static ShaderPtr mShader;
			static GLuint mVAO;

    private:

      void Register();
      //Animation subsystem
      bool AnimationInitialized = false;
      float AnimationSpeedFPS = 0;
      float AnimationSpeedFPSCounter = 0;
			unsigned DrawLayer;
    };

  }
}
