/******************************************************************************/
/*!
@file   SpriteParticleSystem.h
@author Chen Shu, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   11/6/2015
@brief  The SpriteParticleSystem component...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Graphical.h"
#include "../Resources/SpriteSource.h"

namespace DCEngine {

  enum class GeometryMode {
    Billboarded,
    Beam,
    Aligned,
    Outward,
    FaceVelocity,
    Flat,
  };

  namespace Components
  {
    class Transform;
    class Camera;
    class ParticleEmitter;
    class ParticleColorAnimator;
    class LinearParticleAnimator;
    class SpriteParticleSystem : public Graphical
    {
    public:

      // Properties
      Boolean Visible;
      Vec4 Tint = Vec4(1, 1, 1, 1); //!< Color to tint/multiply all particles in the system.
      String Texture; // Allows the use of textures/spritesources
      Real VelocityScale = 1;
      Real LengthScale = 1;
      Vec3 SystemSize = Vec3(4, 4, 4);
      Boolean Additive;
      Boolean Lock;
      DCE_DEFINE_PROPERTY(Boolean, Visible);
      DCE_DEFINE_PROPERTY(Vec4, Tint);
      DCE_DEFINE_PROPERTY(String, Texture);
      DCE_DEFINE_PROPERTY(Real, VelocityScale);
      DCE_DEFINE_PROPERTY(Real, LengthScale);
      DCE_DEFINE_PROPERTY(Vec3, SystemSize);
      DCE_DEFINE_PROPERTY(Boolean, Additive);
      DCE_DEFINE_PROPERTY(Boolean, Lock);
      DCE_DEFINE_PROPERTY(unsigned, DrawLayer);


      ZilchDeclareDerivedType(SpriteParticleSystem, Graphical);
      SpriteParticleSystem(Entity& owner);
      virtual ~SpriteParticleSystem(void);
      void Initialize();
      void Update(float);
      void SetUniforms(ShaderPtr shader, Camera* camera, Light* light);
      void Draw(void);
      Transform* TransformComponent;

      //logic
      void AddParticle(void);
      std::vector<Vec2> GetPositionData(void);
      std::vector<float> GetScaleData(void);
      std::vector<float> GetRotationData(void);
      std::vector<Vec4> GetColorData(void);
      unsigned GetParticleCount(void);

      ParticleEmitter* mParticleEmitter;
      ParticleColorAnimator* mColorAnimator;
      LinearParticleAnimator* mLinearAnimator;

      static GLuint mVAO, mColorInstanceVBO, mTransformInstanceVBO;
      static ShaderPtr mShader;

      // Particles
      class Particle
      {
      public:
        Particle(float, const Vec2&, const Vec2&, const Vec2&, float, float, const Vec4&,
          ParticleColorAnimator*, LinearParticleAnimator*);
        ~Particle();
        static unsigned Created;
        static unsigned Destroyed;

        //Particle& operator=(const Particle&);
        void Update(float);

        float GetLifetime(void) const;
        float GetLifeleft(void) const;
        Vec2 GetPosition(void) const;
        float GetScale(void) const;
        float GetRotation(void) const;
        Vec4 GetColor(void) const;
      private:
        float mLifetime;
        float mLifeleft;
        Vec2 mPosition;
        Vec2 mVelocity;
        Vec2 mAcceleration;
        float mScale;
        float mGrowth;
        float mRotation;
        float mRotationRate;
        float mTorque;
        Vec4 mColor;
        Vec4 mTint;
        ParticleColorAnimator* mColorAnimator;
        LinearParticleAnimator* mLinearAnimator;
      };
      std::list<Particle> mParticleList;
      float mParticleEmissionTimer;
      unsigned mEmitCounter;
      bool mActiveFlag;
      Camera* CameraComponent;
      std::vector<glm::mat4> transformData;
    };
  }

}
