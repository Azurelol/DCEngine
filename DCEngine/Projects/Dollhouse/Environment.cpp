#include "Dollhouse.h"

namespace DCEngine {

  void DollHouse::EnvironmentSetup() {

    GameObjectPtr floor = ConstructGameObject("Floor");
    //floor->AddComponent(ComponentPtr(new Sprite(*floor)));
    //floor->getComponent<Sprite>()->Color = Vec4(1.0f, 0.4f, 0.1f, 1.0f);
    floor->AddComponent(ComponentPtr(new Transform(*floor)));
    floor->AddComponent(ComponentPtr(new RigidBody(*floor)));
    floor->AddComponent(ComponentPtr(new BoxCollider(*floor)));
    //floor->AddComponent(ComponentPtr(new DebugCollider(*floor)));
    floor->AddComponent(ComponentPtr(new DebugReport(*floor)));
    // Change the properties of the components
    floor->getComponent<Transform>()->Translation = Vec3(0.0f, -5.0f, 0.0f);
    floor->getComponent<Transform>()->Scale = Vec3(100.0f, 1.0f, 1.0f);
    // BoxCollider properties
    floor->getComponent<BoxCollider>()->Size = Vec3(100, 2, 2);
    floor->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    floor->getComponent<RigidBody>()->DynamicState = DynamicStateType::Static;


    GameObjectPtr floor2 = ConstructGameObject("Basketfloor2");
    //floor2->AddComponent(ComponentPtr(new Sprite(*floor2)));
   // floor2->getComponent<Sprite>()->Color = Vec4(1.0f, 0.4f, 0.1f, 1.0f);

    floor2->AddComponent(ComponentPtr(new Transform(*floor2)));
    floor2->AddComponent(ComponentPtr(new RigidBody(*floor2)));
    floor2->AddComponent(ComponentPtr(new BoxCollider(*floor2)));
    //floor2->AddComponent(ComponentPtr(new DebugCollider(*floor2)));
    floor2->AddComponent(ComponentPtr(new DebugReport(*floor2)));
    // Change the properties of the components
    floor2->getComponent<Transform>()->Translation = Vec3(0.0f, 20.0f, 0.0f);
    floor2->getComponent<Transform>()->Scale = Vec3(100.0f, 1.0f, 1.0f);
    // Sprite properties
    // BoxCollider properties
    floor2->getComponent<BoxCollider>()->Size = Vec3(100, 2, 2);
    floor2->getComponent<BoxCollider>()->IsDrawingCollider = true;
    // Rigidbody properties
    floor2->getComponent<RigidBody>()->DynamicState = DynamicStateType::Static;

    /* Background Layer */
    GameObjectPtr background = ConstructGameObject("Background");
    background->AddComponent(ComponentPtr(new Transform(*background)));
    background->AddComponent(ComponentPtr(new Sprite(*background)));
    background->getComponent<Sprite>()->SpriteSource = "Background";
    //background->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
    background->getComponent<Transform>()->Translation = Vec3(0, 5, -4);
    background->getComponent<Transform>()->Scale = Vec3(160 * 2, 90 * 2, 1);

    /* Middleground Layer */
    GameObjectPtr middleground = ConstructGameObject("Middleground");
    middleground->AddComponent(ComponentPtr(new Transform(*middleground)));
    middleground->AddComponent(ComponentPtr(new Sprite(*middleground)));
    middleground->getComponent<Sprite>()->SpriteSource = "Middleground";
    //middleground->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
    middleground->getComponent<Transform>()->Translation = Vec3(0, 0, -3);
    middleground->getComponent<Transform>()->Scale = Vec3(160, 90, 1);

    /* Foreground Layer */
    GameObjectPtr foreground = ConstructGameObject("Foreground");
    foreground->AddComponent(ComponentPtr(new Transform(*foreground)));
    foreground->AddComponent(ComponentPtr(new Sprite(*foreground)));
    foreground->getComponent<Sprite>()->SpriteSource = "Foreground";
    //foreground->getComponent<Sprite>()->Color = Vec4(0.6, 0.6, 0.3, 1); // Color: Green
    foreground->getComponent<Transform>()->Translation = Vec3(0, 15.5, 0); //lol
    foreground->getComponent<Transform>()->Scale = Vec3(160 * 0.5, 90 * 0.5, 1);

    /* Particle Tester Layer */
    GameObjectPtr particleTest = ConstructGameObject("particleTest");
    particleTest->AddComponent(ComponentPtr(new Transform(*particleTest)));
    particleTest->AddComponent(ComponentPtr(new SpriteParticleSystem(*particleTest)));
    particleTest->AddComponent(ComponentPtr(new ParticleEmitter(*particleTest)));
    particleTest->AddComponent(ComponentPtr(new LinearParticleAnimator(*particleTest)));
    particleTest->AddComponent(ComponentPtr(new ParticleColorAnimator(*particleTest)));
    // Settings 
    particleTest->getComponent<Transform>()->Translation = Vec3(4, 4, 0);
    particleTest->getComponent<SpriteParticleSystem>()->SpriteSource = "square";
    particleTest->getComponent<ParticleEmitter>()->EmitCount = 6;



  }

}