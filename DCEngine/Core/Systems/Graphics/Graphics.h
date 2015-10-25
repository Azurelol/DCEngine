/*****************************************************************************/
/*!
\file   Graphics.h
\author Christian Sagel, Chen Shu
\par    email: c.sagel\@digipen.edu
\date   9/12/2015
\brief  The graphics system is the bridge between the rendering calls from the 
        entity components and the underlying low-level OpenGL renderer. It
        receives rendering requests and draws on the window.
*/
/******************************************************************************/
#pragma once
#include "../System.h"

// OpenGL rendering
#include "GraphicsGL.h"
#include "../../Components/GraphicsSpace.h"

namespace DCEngine {  
  class Engine;
  class Camera;
  class Sprite;
  class SpriteText;
  class DebugDrawObject;

  namespace Systems {

    class Graphics : public System {
      friend class Engine;
    public:


      void Register(GraphicsSpace& graphicsSpace);
      
      void DrawSprite(Sprite& sprite, Camera& camera, float dt);
      void DrawSpriteText(SpriteText& st, Camera& cam);
      void DrawModel(GameObject& gameObj);
      void DrawDebug(DebugDrawObject& debugDraw);  

      /* Debug Drawing functions*/
      void DrawCircle(Real3& pos, Real& radius, Real4& color, Camera& cam);
      void DrawRectangle(Real3& pos, Real& width, Real& height, Real4& color, Camera& cam);
      void DrawLineSegment(Real3& startPos, Real3& endPos, Real& length, Real4& color, Camera& cam);

    private:

      void StartFrame();
      void EndFrame();

      std::unique_ptr<GraphicsGL> GraphicsHandler;
      const int screenwidth_ = 1024;
      const int screenheight_ = 768;
      Real4 ClearColor = Real4(0.0f, 0.5f, 1.0f, 1.0f);
      Mat4 ProjMatrix;
      Mat4 ViewMatrix;
      Mat4 ViewProjMatrix;
      std::vector<GraphicsSpace*> graphicsSpaces_; //!< Container of graphics spaces accessing this system

      Graphics();
      void Initialize();
      void Update(float dt);
      void Terminate();
      virtual void Serialize(Json::Value& root);
      virtual void Deserialize(Json::Value& root);

	  //2D draw list
	  int TotalObjNumG = 0;
	  int TotalObjTranspNumG = 0;
	  
	  std::vector<Sprite*>  NonTextureObjNontransp;
	  std::vector<Sprite*>  TransparentObj;
	  void SendCountToGL(int TotalObjNumG, int TotalObjTransNumG);
    };


  }

}
