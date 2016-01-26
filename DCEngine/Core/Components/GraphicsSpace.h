/*****************************************************************************/
/*!
\file   GraphicsSpace.h
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  This component receives drawing requests from entities with Sprite,
SpriteText and Model components, and passes the requests to the
engine's underlying graphics system.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"
#include "../Objects/Entities/GameObject.h"
#include "../Objects/DebugDraw.h"

#include "../Components/Sprite.h"
#include "../Components/SpriteText.h"
#include "../Components/SpriteParticleSystem.h"

namespace DCEngine {

  class Graphics;

  namespace Components {

    class Camera;
    class CameraViewport;

    // Containers
    using SpriteTextContainer = std::vector<SpriteText*>;
    using SpriteContainer = std::vector<Sprite*>;
		using GraphicsComponentContainer = std::vector<Graphical*>;

    class GraphicsSpace : public Component {
      friend class Graphics;

    public:

      #if (DCE_USE_ZILCH_INTERNAL_BINDING)
      ZilchDeclareDerivedType(GraphicsSpace, Component);
      #endif

      SpriteLayerOrderHandle SpriteLayerOrder;
      bool Active = true; // Prevent visibility, particle system and render updates
                                
      DCE_DEFINE_PROPERTY(bool, Active);
      DCE_DEFINE_PROPERTY(SpriteLayerOrderHandle, SpriteLayerOrder);

      GraphicsSpace(Entity& owner);
      ~GraphicsSpace();
      void Initialize();

			void RegisterGraphicsComponent(Graphical* graphicsComponent);
			void RemoveGraphicsComponent(Graphical* graphicsComponent);

      //void AddSprite(Sprite& sprite);
      //void RemoveSprite(Sprite& sprite);
      //void AddSpriteText(SpriteText& spriteText);
      //void RemoveSpriteText(SpriteText& spriteText);
			//void AddParticleSystem(SpriteParticleSystem& particleEmitter);
			//void RemoveParticleSystem(SpriteParticleSystem& particleEmitter);

      // Debug Drawing functions 
      void DrawCircle(Vec3& pos, Real& radius, Vec4& color);
      void DrawRectangle(Vec3& pos, Real& width, Real& height, Vec4& color);
      void DrawLineSegment(Vec3& startPos, Vec3& endPos, Vec4& color);

      /* Getters */
      //SpriteContainer getSprites(); // Return the vector of sprites to be drawn
      //SpriteTextContainer getSpriteTextContainer(); // Returns the container of SpriteText to be drawn
			//std::vector<SpriteParticleSystem*> getParticleSystem(); // Returns the emission components

			GraphicsComponentContainer getGraphicsComponents(void);

      void OnLogicUpdate(Events::LogicUpdate* updateEvent);

    private:

      Graphics* GraphicsSystem;
      CameraViewport* CameraViewportComponent;
      GameObjectRawVec models_;  //!< Container of models that need to be rendered
      SpriteContainer SpriteContainer; //!< Container of sprites that need to be drawn    
      SpriteTextContainer SpriteTextContainer;  //!< Container of models that need to be rendered
			std::vector<SpriteParticleSystem*> ParticleSystemContainer;
			GraphicsComponentContainer GraphicsComponents;
      DebugDrawObjVec DebugDrawObjContainer; //!< Container of debug draw objects to be drawn this frame.
    };

  }
}