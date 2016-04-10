/*****************************************************************************/
/*!
@file   Enumerations.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   4/2/2016
@brief  Custom enumerations used by the engine.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.

*/
/******************************************************************************/
#pragma once

namespace DCEngine {

  enum class EnumeratedSystem {
    Window,
    Input,
    Graphics,
    Audio,
    Physics,
    Factory,
    Content,
    Serializer,
    Reflection,
    Editor,
    GUI,
    Keyboard,
    Mouse,
    // The amount of systems
    Capacity
  };
  // Alias for the enum
  using ESys = EnumeratedSystem;

  enum class ResourceType {
    None,
    Archetype,
    Bank,
    CollisionGroup,
    CollisionTable,
    Font,
    Level,
    Material,
    PhysicsMaterial,
    SoundCue,
    SpriteSource,
    SpriteLayer,
    SpriteLayerOrder,
    ZilchScript,
  };

  enum class EntityType {
    GameObject,
    Space,
    GameSession,
  };

  enum class Ease {
    Linear,
    QuadIn,
    QuadInOut,
    QuadOut,
    SinIn,
    SinInOut,
    SinOut,
  };

  enum class Direction {
    None,
    Up,
    Down,
    Left,
    Right,
  };

  enum class StopMode {
    Immediate,
    AllowFadeOut
  };

  enum class Keys {
    None,
    Tilde,
    Tab,
    Escape,
    Space,
    Return,
    Backspace,
    Delete,
    LControl,
    RControl,
    LShift,
    RShift,
    Up,
    Down,
    Left,
    Right,

    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,

    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,

    Z,
    X,
    C,
    V,
    B,
    N,
    M,

    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Add,
    Subtract,

    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
  };

  enum class MouseButton {
    Left,
    Middle,
    Right
  };

}