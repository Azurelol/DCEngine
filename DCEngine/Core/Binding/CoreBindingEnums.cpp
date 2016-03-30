/******************************************************************************/
/*!
@file   CoreBindingEvents.cpp
@author Gabriel Neumann
@par    email: g.neumann\@digipen.edu
@date   2/27/2016
@brief  Binds enums used in the engine so that Zilch can convert to them and
back.
@copyright Copyright 2016, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "CoreBindingEnums.h"


ZilchDefineExternalType(DCEngine::Direction, "Direction", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::Direction::None, "None");
  ZilchBindEnumValue(builder, type, DCEngine::Direction::Up, "Up");
  ZilchBindEnumValue(builder, type, DCEngine::Direction::Down, "Down");
  ZilchBindEnumValue(builder, type, DCEngine::Direction::Left, "Left");
  ZilchBindEnumValue(builder, type, DCEngine::Direction::Right, "Right");
}

ZilchDefineExternalType(DCEngine::Ease, "Ease", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::Ease::Linear, "Linear");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::QuadIn, "QuadIn");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::QuadOut, "QuadOut");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::QuadInOut, "QuadInOut");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::SinIn, "SinIn");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::SinOut, "SinOut");
  ZilchBindEnumValue(builder, type, DCEngine::Ease::SinInOut, "SinInOut");
}

ZilchDefineExternalType(DCEngine::Keys, "Keys", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::Keys::None, "None");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Tilde, "Tilde");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Tab, "Tab");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Escape, "Escape");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Space, "Space");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Return, "Return");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Backspace, "Backspace");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Delete, "Delete");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::LControl, "LControl");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::RControl, "RControl");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::LShift, "LShift");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::RShift, "RShift");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Up, "Up");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Down, "Down");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Left, "Left");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Right, "Right");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Q, "Q");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::W, "W");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::E, "E");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::R, "R");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::T, "T");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Y, "Y");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::U, "U");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::I, "I");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::O, "O");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::P, "P");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::A, "A");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::S, "S");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::D, "D");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F, "F");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::G, "G");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::H, "H");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::J, "J");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::K, "K");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::L, "L");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Z, "Z");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::X, "X");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::C, "C");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::V, "V");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::B, "B");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::N, "N");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::M, "M");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num1, "Num1");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num2, "Num2");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num3, "Num3");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num4, "Num4");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num5, "Num5");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num6, "Num6");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num7, "Num7");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num8, "Num8");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Num9, "Num9");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Add, "Add");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::Subtract, "Subtract");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F1, "F1");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F2, "F2");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F3, "F3");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F4, "F4");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F5, "F5");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F6, "F6");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F7, "F7");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F8, "F8");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F9, "F9");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F10, "F10");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F11, "F11");
  ZilchBindEnumValue(builder, type, DCEngine::Keys::F12, "F12");
}

ZilchDefineExternalType(DCEngine::MouseButton, "MouseButton", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::MouseButton::Left, "Left");
  ZilchBindEnumValue(builder, type, DCEngine::MouseButton::Middle, "Middle");
  ZilchBindEnumValue(builder, type, DCEngine::MouseButton::Right, "Right");
}

ZilchDefineExternalType(DCEngine::MouseWheelScrollDirection, "MouseWheelScrollDirection", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::MouseWheelScrollDirection::Up, "Up");
  ZilchBindEnumValue(builder, type, DCEngine::MouseWheelScrollDirection::Down, "Down");
}

ZilchDefineExternalType(DCEngine::CollisionFlag, "CollisionFlag", DCEngine::DCEngineCore, builder, type)
{
  ZilchBindEnum(builder, type, SpecialType::Enumeration);
  ZilchBindEnumValue(builder, type, DCEngine::CollisionFlag::SkipDetecting, "SkipDetecting");
  ZilchBindEnumValue(builder, type, DCEngine::CollisionFlag::SkipResolution, "SkipResolution");
  ZilchBindEnumValue(builder, type, DCEngine::CollisionFlag::Resolve, "Resolve");
}