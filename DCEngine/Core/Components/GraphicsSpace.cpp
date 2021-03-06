/*****************************************************************************/
/*!
\file   GraphicsSpace.cpp
\author Christian Sagel
\par    email: c.sagel\@digipen.edu
\date   9/16/2015
\brief  This component receives drawing requests from entities with Sprite,
SpriteText and Model components, and passes the requests to the
engine's underlying graphics system.

@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "GraphicsSpace.h"

#include "EngineReference.h"
#include "../Systems/Graphics/Graphics.h" // Access to the graphics system

namespace DCEngine {

  namespace Components
  {
    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(GraphicsSpace, "GraphicsSpace", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(GraphicsSpace);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(SpriteLayerOrder);
      // Properties
      DCE_BINDING_DEFINE_PROPERTY(GraphicsSpace, SpriteLayerOrder);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertySpriteLayerOrder, attributeSpriteLayerOrder);
      DCE_BINDING_DEFINE_PROPERTY(GraphicsSpace, Active);
      ZilchBindMethod(builder, type, &GraphicsSpace::SetResolution, ZilchNoOverload, "SetResolution", "x, y");
      ZilchBindMethod(builder, type, &GraphicsSpace::SetAntiAliasingLevel, ZilchNoOverload, "SetAntiAliasingLevel", "samples");
      ZilchBindMethod(builder, type, &GraphicsSpace::ToggleFullscreen, ZilchNoOverload, "ToggleFullscreen", ZilchNoNames);
      ZilchBindMethod(builder, type, &GraphicsSpace::SetNative, ZilchNoOverload, "SetNative", ZilchNoNames);
      //ZilchBindProperty(builder, type, &RigidBody::getDynamicState, &RigidBody::setDynamicState, "DynamicState");
    }
    #endif

    /**************************************************************************/
    /*!
    \brief Default GraphicsSpace constructor.
    */
    /**************************************************************************/
    GraphicsSpace::GraphicsSpace(Entity & owner) : Component(std::string("GraphicsSpace"), owner)
      , Active(true)
    {
      SpriteLayerOrder = SpriteLayerOrder::Default();
    }

    /**************************************************************************/
    /*!
    \brief Default GraphicsSpace destructor.
    */
    /**************************************************************************/
    GraphicsSpace::~GraphicsSpace() {
      // Deregister from the Graphics System
      Daisy->getSystem<Systems::Graphics>()->Deregister(*this);
    }

    /**************************************************************************/
    /*!
    \brief Initializes the GraphicsSpace component.
    */
    /**************************************************************************/
    void GraphicsSpace::Initialize() {
      // Store a reference to the space's 'CameraViewport' component
      CameraViewportComponent = Owner()->getComponent<Components::CameraViewport>();
      // Register this space to the graphics system
      Daisy->getSystem<Systems::Graphics>()->Register(*this);
    }

    void GraphicsSpace::RegisterGraphicsComponent(Graphical * graphicsComponent)
    {
      GraphicsComponents.push_back(graphicsComponent);
    }
    void GraphicsSpace::RemoveGraphicsComponent(Graphical * graphicsComponent)
    {
      GraphicsComponents.erase(std::remove(GraphicsComponents.begin(),
        GraphicsComponents.end(), graphicsComponent), GraphicsComponents.end());
    }
    GraphicsComponentContainer GraphicsSpace::getGraphicsComponents(void)
    {
      return GraphicsComponents;
    }

    void GraphicsSpace::RegisterLightComponent(Light * lightComponent)
    {
      LightComponents.push_back(lightComponent);
    }

    void GraphicsSpace::RemoveLightComponent(Light * lightComponent)
    {
      LightComponents.erase(std::remove(LightComponents.begin(),
        LightComponents.end(), lightComponent), LightComponents.end());
    }

    LightComponentContainer GraphicsSpace::getLightComponents(void)
    {
      return LightComponents;
    }

    void GraphicsSpace::SetResolution(unsigned x, unsigned y)
    {
      Daisy->getSystem<Systems::Graphics>()->SetResolution(x, y);
    }
    void GraphicsSpace::SetAntiAliasingLevel(unsigned samples)
    {
      Daisy->getSystem<Systems::Graphics>()->SetAntiAliasingLevel(samples);
    }
    void GraphicsSpace::ToggleFullscreen()
    {
      Daisy->getSystem<Systems::Graphics>()->ToggleFullscreen();
    }
    unsigned GraphicsSpace::GetScreenWidth() const
    {
      return GraphicsSystem->GetScreenWidth();
    }
    unsigned GraphicsSpace::GetScreenHeight() const
    {
      return GraphicsSystem->GetScreenHeight();
    }
    void GraphicsSpace::SetNative()
    {
      DCEngine::Systems::DispatchSystemEvents::WindowResizeToNative();
    }

    /**************************************************************************/
    /*!
    \brief Registers a SpriteText into the GraphicsSpace
    */
    /**************************************************************************/
    //void GraphicsSpace::AddSpriteText(SpriteText& spriteText)
    //{
    //  SpriteTextContainer.push_back(&spriteText);
    //  //DCTrace << "GraphicsSpace::AddSpriteText - "
    //  //  << spriteText.Owner()->Name() << "\n";
    //}
    //
    ///**************************************************************************/
    ///*!
    //@brief  Registers a Sprite into the GraphicsSpace
    //@param  A reference to an instance of the 'Sprite' component.
    //*/
    ///**************************************************************************/
    //void GraphicsSpace::AddSprite(Sprite & sprite)
    //{
    //  SpriteContainer.push_back(&sprite);
    //  //DCTrace << Owner()->Name() << "GraphicsSpace::AddSprite - "
    //  //        << sprite.Owner()->Name() << "\n";
    //}
    //
    ///**************************************************************************/
    ///*!
    //@brief  Deregisters a Sprite from the GraphicsSpace
    //@param  A reference to an instance of the 'Sprite' component.
    //*/
    ///**************************************************************************/
    //void GraphicsSpace::RemoveSprite(Sprite & sprite)
    //{
    //  //DCTrace << Owner()->Name() <<"::GraphicsSpace::RemoveSprite - "
    //  //        << sprite.Owner()->Name() << "\n";
    //  Sprite* spritePtr = &sprite;
    //  SpriteContainer.erase(std::remove(SpriteContainer.begin(),
    //    SpriteContainer.end(), spritePtr),
    //    SpriteContainer.end());
    //}
    //
    ///**************************************************************************/
    ///*!
    //@brief  Deregisters a SpriteText from the GraphicsSpace
    //@param  A reference to an instance of the 'SpriteText' component.
    //*/
    ///**************************************************************************/
    //void GraphicsSpace::RemoveSpriteText(SpriteText & spriteText)
    //{
    //  SpriteText* spriteTextPtr = &spriteText;
    //  SpriteTextContainer.erase(std::remove(SpriteTextContainer.begin(),
    //                            SpriteTextContainer.end(), spriteTextPtr),
    //                            SpriteTextContainer.end());
    //}
    //
    //void GraphicsSpace::AddParticleSystem(SpriteParticleSystem & particleEmitter)
    //{
    //	ParticleSystemContainer.push_back(&particleEmitter);
    //}
    //
    //void GraphicsSpace::RemoveParticleSystem(SpriteParticleSystem & particleEmitter)
    //{
    //	ParticleSystemContainer.erase(std::remove(ParticleSystemContainer.begin(),
    //		ParticleSystemContainer.end(), &particleEmitter), ParticleSystemContainer.end());
    //}


    /**************************************************************************/
    /*!
    @brief Draws a circle at the specified position.
    @param pos The center of the circle.
    @param radius The radius of the circle.
    @param color The color of the circle.
    @param fill Whether the circle should be filled.
    */
    /**************************************************************************/
    void GraphicsSpace::DrawCircle(const Vec3& pos, Real radius, const Vec4& color, bool fill) const
    {
      if (!CameraViewportComponent)
        return;

      auto cam = CameraViewportComponent->getCamera();
      if (!cam) return;

      Daisy->getSystem<Systems::Graphics>()->DrawCircle(pos, radius, color, *cam, fill);
    }

    /**************************************************************************/
    /*!
    @brief Draws a rectangle at the specified position.
    @param pos The center of the rectangle.
    @param radius The width of the rectangle.
    @param height The height of the rectangle.
    @param color The color of the rectangle.
    @param fill Whether the rectangle should be filled.
    */
    /**************************************************************************/
    void GraphicsSpace::DrawRectangle(const Vec3& pos, Real width, Real height, const Vec4& color, bool fill) const
    {
      if (!CameraViewportComponent)
        return;

      auto cam = CameraViewportComponent->getCamera();
      if (!cam) return;

      Daisy->getSystem<Systems::Graphics>()->DrawRectangle(pos, width, height, color, *cam, fill);
    }

    /**************************************************************************/
    /*!
    @brief Draws a line from the specified starting position to the end position.
    @param startPos The starting position of the line.
    @param endPos The ending position of the line.
    @param color  The color of the line.
    */
    /**************************************************************************/
    void GraphicsSpace::DrawLineSegment(const Vec3& startPos, const Vec3& endPos, const Vec4& color) const
    {
      if (!CameraViewportComponent)
        return;

      auto cam = CameraViewportComponent->getCamera();
      if (!cam) return;

      Daisy->getSystem<Systems::Graphics>()->DrawLineSegment(startPos, endPos, color, *cam);
    }




    /**************************************************************************/
    /*!
    \brief  Returns the container of Sprite pointers.
    \return A container of Sprite pointers.
    */
    /**************************************************************************/
    //std::vector<Sprite*> GraphicsSpace::getSprites() {
    //  return SpriteContainer;
    //}
    //
    //SpriteTextContainer GraphicsSpace::getSpriteTextContainer()
    //{
    //  return SpriteTextContainer;
    //}
    //
    //std::vector<SpriteParticleSystem*> GraphicsSpace::getParticleSystem()
    //{
    //	return ParticleSystemContainer;
    //}

  }
}