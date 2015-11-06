/******************************************************************************/
/*!
@file   ComponentsInclude.h
@author Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/6/2015
@brief  A header containing all of the core engine components.
*/
/******************************************************************************/
#pragma once

/* Space-specific components */
#include "Components\PhysicsSpace.h"
#include "Components\GraphicsSpace.h"
#include "Components\SoundSpace.h"

/* Graphics */
#include "Components\Camera.h"
#include "Components\CameraViewport.h"
#include "Components\Sprite.h"
#include "Components\SpriteText.h"
// Particles
#include "Components\SpriteParticleSystem.h"
#include "Components\ParticleEmitter.h"
#include "Components\ParticleColorAnimator.h"
#include "Components\LinearParticleAnimator.h"

/* Physics */
#include "Components\Transform.h"
#include "Components\RigidBody.h"
#include "Components\BoxCollider.h"
#include "Components\Orientation.h"
#include "Components\IgnoreSpaceEffects.h"

/* Debug */
#include "Components\DebugMoveController.h"
#include "Components\DebugReport.h"
#include "Components\DebugCamera.h"
#include "Components\DebugCollider.h"
#include "Components\DebugAudio.h"
#include "Components\DebugFade.h"

/* Project Components */
#include "../Projects/ProjectComponents.h"

