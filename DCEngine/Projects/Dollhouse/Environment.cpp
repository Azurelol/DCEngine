#include "Dollhouse.h"

namespace DCEngine {

  void DollHouse::EnvironmentSetup() {

    GameObjectPtr floor = ConstructGameObject("Floor");
    //floor->AddComponent(ComponentPtr(new Sprite(*floor)));
    //floor->getComponent<Sprite>()->Color = Vec4(1.0f, 0.4f, 0.1f, 1.0f);
    floor->AddComponent<Transform>();
    floor->AddComponent<RigidBody>();
    floor->AddComponent<BoxCollider>();
    //floor->AddComponent(ComponentPtr(new DebugCollider(*floor)));
    floor->AddComponent<DebugReport>();
    // Change the properties of the components
    floor->getComponent<Transform>()->Translation = Vec3(0.0f, -5.0f, 0.0f);
    floor->getComponent<Transform>()->Scale = Vec3(100.0f, 1.0f, 1.0f);
    // BoxCollider properties
    floor->getComponent<BoxCollider>()->Size = Vec3(100, 2, 2);
    floor->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    floor->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);


    GameObjectPtr floor2 = ConstructGameObject("Basketfloor2");
    //floor2->AddComponent(ComponentPtr(new Sprite(*floor2)));
   // floor2->getComponent<Sprite>()->Color = Vec4(1.0f, 0.4f, 0.1f, 1.0f);

    floor2->AddComponent<Transform>();
    floor2->AddComponent<RigidBody>();
    floor2->AddComponent<BoxCollider>();
    //floor2->AddComponent(ComponentPtr(new DebugCollider(*floor2)));
    floor2->AddComponent<DebugReport>();
    // Change the properties of the components
    floor2->getComponent<Transform>()->Translation = Vec3(0.0f, 20.0f, 0.0f);
    floor2->getComponent<Transform>()->Scale = Vec3(100.0f, 1.0f, 1.0f);
    // Sprite properties
    // BoxCollider properties
    floor2->getComponent<BoxCollider>()->Size = Vec3(100, 2, 2);
    floor2->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    floor2->getComponent<RigidBody>()->setDynamicState(DynamicStateType::Static);

    
    {
      // What
      auto bgScale = 1.5;
      auto bgY = 30;

      /* Sky Layer */
      GameObjectPtr sky = ConstructGameObject("Sky");
      sky->AddComponent<Transform>();
      sky->AddComponent<Sprite>();
      sky->getComponent<Sprite>()->SpriteSource = "owSky";
      //sky->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
      sky->getComponent<Transform>()->Translation = Vec3(0, bgY, -4.01); // lol
      sky->getComponent<Transform>()->Scale = Vec3(160 * bgScale, 90 * bgScale, 1);

      /* Background Layer */
      GameObjectPtr background = ConstructGameObject("Background");
      background->AddComponent<Transform>();
      background->AddComponent<Sprite>();
      background->getComponent<Sprite>()->SpriteSource = "owBackground";
      //background->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
      background->getComponent<Transform>()->Translation = Vec3(0, bgY, -4);
      background->getComponent<Transform>()->Scale = Vec3(160 * bgScale, 90 * bgScale, 1);

      /* Middleground Layer */
      //GameObjectPtr middleground = ConstructGameObject("Middleground");
      //middleground->AddComponent(ComponentPtr(new Transform(*middleground)));
      //middleground->AddComponent(ComponentPtr(new Sprite(*middleground)));
      //middleground->getComponent<Sprite>()->SpriteSource = "Middleground";
      ////middleground->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
      //middleground->getComponent<Transform>()->Translation = Vec3(0, 0, -3);
      //middleground->getComponent<Transform>()->Scale = Vec3(160, 90, 1);

      ///* Foreground Layer */
      //GameObjectPtr foreground = ConstructGameObject("Foreground");
      //foreground->AddComponent(ComponentPtr(new Transform(*foreground)));
      //foreground->AddComponent(ComponentPtr(new Sprite(*foreground)));
      //foreground->getComponent<Sprite>()->SpriteSource = "Foreground";
      ////foreground->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
      //foreground->getComponent<Transform>()->Translation = Vec3(0, 15.5, 0); //lol
      //foreground->getComponent<Transform>()->Scale = Vec3(160 * 0.5, 90 * 0.5, 1);
    }


    /* Particle Tester Layer */
    GameObjectPtr particleTest = ConstructGameObject("particleTest");
    particleTest->AddComponent<Transform>();
    particleTest->AddComponent<SpriteParticleSystem>();
    particleTest->AddComponent<ParticleEmitter>();
    particleTest->AddComponent<LinearParticleAnimator>();
    particleTest->AddComponent<ParticleColorAnimator>();
    // Settings 
    particleTest->getComponent<Transform>()->Translation = Vec3(4, 4, 0);
    particleTest->getComponent<SpriteParticleSystem>()->SpriteSource = "square";
    particleTest->getComponent<ParticleEmitter>()->EmitCount = 6;



  }

}